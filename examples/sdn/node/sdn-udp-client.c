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
#include "lib/random.h"
#include "lib/memb.h"
#include "lib/list.h"
#include "sys/node-id.h"

#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"

#if UIP_CONF_IPV6_SDN
#include "net/sdn/sdn.h"
#include "net/sdn/sdn-ft.h"
#include "net/sdn/sdn-conf.h"
#include "net/sdn/sdn-cd.h"
#include "net/sdn/sdn-packetbuf.h"
#include "net/sdn/usdn/usdn.h"
#endif

#if WITH_SDN_STATS
#include "net/sdn/sdn-stats.h"
#endif

#ifdef BUILD_WITH_MULTIFLOW
#include "multiflow.h"
#endif

#include "net/routing/routing.h"
#if ROUTING_CONF_RPL_LITE
#include "net/routing/rpl-lite/rpl.h"
#include "net/routing/rpl-lite/rpl-types.h"
#endif /* ROUTING_CONF_RPL_LITE */
#if ROUTING_CONF_RPL_CLASSIC
#include "net/routing/rpl-classic/rpl.h"
#include "net/routing/rpl-classic/rpl-private.h"
#endif /* ROUTING_CONF_RPL_CLASSIC */

#include "sys/log.h"
#define LOG_MODULE "NODE"
#define LOG_LEVEL LOG_LEVEL_INFO

/* mflow */
#ifdef BUILD_WITH_MULTIFLOW

#define MAX_PAYLOAD         64

#ifdef CONF_TX_NODES
  #define TX_NODES          CONF_TX_NODES
#else
  #define TX_NODES          0
#endif
#ifdef CONF_RX_NODES
  #define RX_NODES          CONF_RX_NODES
#else
  #define RX_NODES          1
#endif
#ifndef CONF_NUM_APPS
  #define CONF_NUM_APPS     1
#endif
#ifndef CONF_APP_BR_DELAY
  #define CONF_APP_BR_DELAY 120
#endif
#ifndef CONF_APP_BR_MIN
  #define CONF_APP_BR_MIN   60
#endif
#ifndef CONF_APP_BR_MAX
  #define CONF_APP_BR_MAX   75
#endif

#define UDP_RPORT	8765
#define UDP_LPORT	5678

static int app_tx_node[CONF_NUM_APPS] = {TX_NODES};
static int app_rx_node[CONF_NUM_APPS] = {RX_NODES};
static float app_br_delay[CONF_NUM_APPS] = {CONF_APP_BR_DELAY};
static float app_br_min[CONF_NUM_APPS] = {CONF_APP_BR_MIN};
static float app_br_max[CONF_NUM_APPS] = {CONF_APP_BR_MAX};
static mflow_t *apps[CONF_NUM_APPS];

#if SERVER_REPLY
static int reply = 0;                          // keep track of server replies
#endif /* SERVER_REPLY */

#endif /* BUILD_WITH_MULTIFLOW */

PROCESS(sdn_node_process, "SDN Node");
AUTOSTART_PROCESSES(&sdn_node_process);

/*---------------------------------------------------------------------------*/
/* Send function */
/*---------------------------------------------------------------------------*/
#ifdef BUILD_WITH_MULTIFLOW
static void
send(mflow_t *a)
{
  uint8_t buf[MAX_PAYLOAD] = {0};
  int pos = 0;
#if UIP_CONF_IPV6_SDN
  /* check if we are connected to the sdn controller, and we have a flowtable
     entry for the destination. If we don't then we need to query */
  if(sdn_connected()) {
    if(sdn_ft_contains(&a->remote_addr, sizeof(uip_ipaddr_t))) {
#endif /* UIP_CONF_IPV6_SDN */

      /* check we have a rpl dag */
      rpl_dag_t *dag = rpl_get_any_dag();
      if(dag != NULL) { /* Should never be NULL */
#if UIP_CONF_IPV6_SDN
        usdn_hdr_t hdr;
        hdr.net = SDN_CONF.sdn_net;
        hdr.typ = USDN_MSG_CODE_DATA;
        hdr.flow = a->id;
        memcpy(&buf, &hdr, USDN_H_LEN);
        pos += USDN_H_LEN;
#endif /* UIP_CONF_IPV6_SDN */
        sprintf((char *)buf + pos, "a:%d id:%d", a->id, a->seq);
        /*TX APP src dest appid appseq */
        LOG_STAT("TX APP s:%d d:%d %s\n",
          node_id,
          a->remote_addr.u8[15],
          (buf + pos));
          mflow_send(a, &buf, strlen((char *)(buf + pos)) + pos);
      }

#if UIP_CONF_IPV6_SDN
    } else {
      // /* Send a FTQ to the controller to find a path */
      controller_query_data(&a->remote_addr, sizeof(uip_ipaddr_t));
    }
  } else {
    LOG_WARN("app %d can't send, no controller\n", a->id);
  }
#endif /* UIP_CONF_IPV6_SDN */
}

#endif /* BUILD_WITH_MULTIFLOW */

/*---------------------------------------------------------------------------*/
/* App callback functions */
/*---------------------------------------------------------------------------*/
#ifdef BUILD_WITH_MULTIFLOW
static void
app_rx_callback(mflow_t *a,
                const uip_ipaddr_t *source_addr,
                uint16_t src_port,
                const uip_ipaddr_t *dest_addr,
                uint16_t dest_port,
                const uint8_t *data, uint16_t datalen)
{
#if SERVER_REPLY
  reply++;
  if(a->seq > 0) {
    char *color;
    if (reply == a->seq) {
      color = "GREEN";
    } else if (reply >= a->seq - (a->seq*0.1)) {
      color = "ORANGE";
    } else {
      color = "RED";
    }
    LOG_ANNOTATE("#A r=%d/%d,color=%s\n", reply, a->seq, color);
  }
#endif /* SERVER_REPLY */
  LOG_STAT("RX APP s:%d d:%d %s h:%d\n",
    source_addr->u8[15],
    node_id,
    (char *)data,
    uip_ds6_if.cur_hop_limit - UIP_IP_BUF->ttl + 1);
}
#endif /* BUILD_WITH_MULTIFLOW */

/*---------------------------------------------------------------------------*/
#ifdef BUILD_WITH_MULTIFLOW
static void
app_timer_callback(mflow_t *a) {
  /* Reset app timer */
  mflow_new_sendinterval(a, 0);
  /* Send data */
  send(a);
}
#endif /* BUILD_WITH_MULTIFLOW */

/*---------------------------------------------------------------------------*/
/* Initialization */
/*---------------------------------------------------------------------------*/
static void
init(void)
{
/* Spit out SDN statistics */
#if WITH_SDN_STATS
  sdn_stats_start(CLOCK_SECOND * SDN_STATS_PERIOD);
#endif

  uip_ipaddr_t ipaddr;
  /* Add global address from MAC */
  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

#if UIP_CONF_IPV6_SDN
  /* Configure SDN */
  SDN_DRIVER.add_accept_on_icmp6_type(WHITELIST, ICMP6_RPL); /* Accept RPL ICMP messages */
#endif /* UIP_CONF_IPV6_SDN */

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

#ifdef BUILD_WITH_MULTIFLOW
  int i;
  /* Initialise multiflow app process */
  mflow_init();

  /* Allocate some applications */
  for (i = 0; i < CONF_NUM_APPS; i++) {
    uip_ipaddr_t target_addr;
    uip_ipaddr_copy(&target_addr, &uip_ds6_get_link_local(-1)->ipaddr);
    if (app_tx_node[i] == node_id || app_tx_node[i] == 0) {
      target_addr.u8[15] = app_rx_node[i];
      LOG_DBG("Adding new mflow source to ");
      LOG_DBG_6ADDR(&target_addr);
      LOG_DBG_("\n");
      apps[i] = mflow_new(i, &target_addr, UDP_LPORT, UDP_RPORT,
                             app_br_delay[i], app_br_min[i], app_br_max[i],
                             app_rx_callback,
                             app_timer_callback);
      /* Start app timers */
      mflow_new_sendinterval(apps[i], 1);
    } else if (app_rx_node[i] == node_id){
      target_addr.u8[15] = app_tx_node[i];
      LOG_DBG("Adding new mflow source from ");
      LOG_DBG_6ADDR(&target_addr);
      LOG_DBG_("\n");
      apps[i] = mflow_new(i, &target_addr, UDP_RPORT, UDP_LPORT,
                             app_br_delay[i], app_br_min[i], app_br_max[i],
                             app_rx_callback,
                             app_timer_callback);
    } else {
      LOG_ERR("We are not a RX or TX node???\n");
    }
  }
#endif /* BUILD_WITH_MULTIFLOW */
}

/*---------------------------------------------------------------------------*/
/* Printing */
/*---------------------------------------------------------------------------*/
// static void
// print_int_array(int *array, int length)
// {
//   int i;
//   printf("[");
//   for (i = 0; i < length; i++) {
//     printf("%d ", array[i]);
//   }
//   printf("]");
// }

/*---------------------------------------------------------------------------*/
#if BUILD_WITH_MULTIFLOW & (LOG_LEVEL >= LOG_LEVEL_INFO)
static void
print_float_array(float *array, int length)
{
  int i;
  printf("[");
  for (i = 0; i < length; i++) {
    printf("%d.%02lu ", (int)(array[i]),
    (int)(100L * array[i]) - ((int)(array[i])*100L));
  }
  printf("]");
}
#endif /* BUILD_WITH_MULTIFLOW */

/*---------------------------------------------------------------------------*/
/* Prints any RUN_CONF arguments */
void
print_sim_info(void)
{
  LOG_INFO("SDN SETTINGS ...\n");
  LOG_INFO("NSU_FREQ=%d\n", DEFAULT_CONTROLLER->update_period);
  LOG_INFO("FT_LIFETIME=%d\n", (int)(SDN_CONF.ft_lifetime / CLOCK_SECOND));
#if BUILD_WITH_MULTIFLOW & (LOG_LEVEL >= LOG_LEVEL_INFO)
  LOG_INFO("MULTIFLOW SETTINGS ...\n");
  LOG_INFO("NUM_APPS=%d\n", CONF_NUM_APPS);
  LOG_INFO("BR_DELAY=");
  print_float_array(app_br_delay, CONF_NUM_APPS);
  LOG_INFO_(" BR_MIN=");
  print_float_array(app_br_min, CONF_NUM_APPS);
  LOG_INFO_(" BR_MAX=");
  print_float_array(app_br_max, CONF_NUM_APPS);
  LOG_INFO_("\n");
#endif /* BUILD_WITH_MULTIFLOW  */

  /* Print other node information */
  LOG_INFO("Node started! max_nbr:%d\n", NBR_TABLE_CONF_MAX_NEIGHBORS);
}

/*---------------------------------------------------------------------------*/
/* PROCESS */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sdn_node_process, ev, data)
{
  /* Make node random. Stuff above PROCESS_BEGIN is called each time we
     come back to the pt after a yield */
  random_init(node_id);

  PROCESS_BEGIN();
  LOG_INFO("Starting sdn_node_process\n");
  /* Initial configuration */
#if UIP_CONF_IPV6_SDN
  sdn_init();
#endif /* UIP_CONF_IPV6_SDN */
  init();
  /* Print simulation arguments */
  print_sim_info();
  while(1) {
    PROCESS_YIELD();
    if (ev == tcpip_event) {
      LOG_INFO("tcpip_event in PROCESS\n");
    } else {
      LOG_ERR("Unknown event type! %02x\n", ev);
    }
  }

  LOG_INFO("Ending sdn_node_process\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
