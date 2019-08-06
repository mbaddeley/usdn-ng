/*
 * Copyright (c) 2018, Toshiba Research Europe Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/**
 * \file
 *         uSDN: A low-overhead SDN stack and embedded SDN controller for Contiki.
 *         See "Evolving SDN for Low-Power IoT Networks", IEEE NetSoft'18
 * \author
 *         Michael Baddeley <m.baddeley@bristol.ac.uk>
 */
#include "contiki.h"
#include "node-id.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
// #include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/simple-udp.h"

#include "sys/node-id.h"
#include "sys/ctimer.h"
#include "lib/random.h"

#include "net/routing/routing.h"
#if ROUTING_CONF_RPL_LITE
#include "net/routing/rpl-lite/rpl.h"
#include "net/routing/rpl-lite/rpl-types.h"
#endif /* ROUTING_CONF_RPL_LITE */
#if ROUTING_CONF_RPL_CLASSIC
#include "net/routing/rpl-classic/rpl.h"
#include "net/routing/rpl-classic/rpl-private.h"
#endif /* ROUTING_CONF_RPL_CLASSIC */

#if UIP_CONF_IPV6_SDN
#include "net/sdn/sdn.h"
#include "net/sdn/sdn-conf.h"
#include "net/sdn/usdn/usdn.h"
#endif

#if BUILD_WITH_ATOM
#include "atom.h"
#endif

#if WITH_SDN_STATS
#include "net/sdn/sdn-stats.h"
#endif

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "NODE"
#define LOG_LEVEL LOG_LEVEL_DBG

#define UDP_LPORT	8765
#define UDP_RPORT	5678

#define CONTROLLER_LPORT 1234
#define CONTROLLER_RPORT 4321

static struct simple_udp_connection udp_conn;

PROCESS(sdn_controller_process, "SDN Controller");
AUTOSTART_PROCESSES(&sdn_controller_process);

/*---------------------------------------------------------------------------*/
static void
configure_sdn() {
  uip_ipaddr_t ipaddr;

  LOG_INFO("Configuring SDN...\n");

  sdn_init();
  /* Construct a SDN Controller IP Address. */
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 1);
  /* Initialise Atom */
  atom_init(&ipaddr);

  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0x200, 0, 0, 1);
  SDN_DRIVER.add_accept_on_src(FLOWTABLE, &ipaddr);          /* Ensure our own outbound packets aren't checked by SDN */
  SDN_DRIVER.add_accept_on_icmp6_type(FLOWTABLE, ICMP6_RPL); /* Accept RPL ICMP messages */
}

/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  char buf[15] = {0};
  int pos = 0;

  usdn_hdr_t hdr;
  memcpy(&hdr, data, USDN_H_LEN);
  pos += USDN_H_LEN;

  memcpy(&buf, data + pos, datalen);
  LOG_STAT("RX APP s:%d d:%d %s h:%d\n",
    sender_addr->u8[15],
    node_id,
    (char *)buf,
    uip_ds6_if.cur_hop_limit - UIP_IP_BUF->ttl + 1);
#if SERVER_REPLY
  LOG_STAT("Sending response %s to %u\n",
    (char *)buf,
    sender_addr->u8[15]);
  simple_udp_sendto(&udp_conn, data, datalen, sender_addr);
#endif /* SERVER_REPLY */
}
/*-------------------------------------------FTS--------------------------------*/
PROCESS_THREAD(sdn_controller_process, ev, data)
{
  PROCESS_BEGIN();
  LOG_INFO("Starting sdn_controller_process\n");

  LOG_INFO("Configuring RPL...\n");
  /* Initialize DAG root */
  NETSTACK_ROUTING.root_start();

  /* Initialize SDN */
  configure_sdn();

  uip_ds6_addr_t *lladdr;
  uip_ds6_addr_t* global;
  lladdr = uip_ds6_get_link_local(-1);
  global = uip_ds6_get_global(-1);

  LOG_DBG("Local: ");
  LOG_INFO_6ADDR(lladdr != NULL ? &lladdr->ipaddr : NULL);
  LOG_DBG_("\n");
  LOG_DBG("Global: ");
  LOG_INFO_6ADDR(global != NULL ? &global->ipaddr : NULL);
  LOG_DBG_("\n");

  /* Spit out SDN statistics */
#if WITH_SDN_STATS
  sdn_stats_start(CLOCK_SECOND * SDN_STATS_PERIOD);
#endif

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_LPORT, NULL,
                      UDP_RPORT, udp_rx_callback);

  LOG_DBG("Forcing controller update!\n");
  SDN_ENGINE.controller_update(SDN_TMR_STATE_IMMEDIATE);

    /* Main loop for events */
  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      LOG_INFO("Received tcpip_event\n");
    }
  }

  LOG_INFO("Ending sdn_controller_process\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
