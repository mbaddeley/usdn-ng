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
 *     without specific prior written permission.
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
 *         uSDN Core: SDN configuration for nodes.
 * \author
 *         Michael Baddeley <m.baddeley@bristol.ac.uk>
 */
#include "net/sdn/sdn.h"
#include "net/sdn/sdn-conf.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "SDN-CONF"
#define LOG_LEVEL LOG_LEVEL_SDN

/*----------------------------------------------------------------------------*/
sdn_cfg_t SDN_CONF;

/*----------------------------------------------------------------------------*/
void
sdn_conf_init(void)
{
  /* Basic configuration */
  SDN_CONF.sdn_net =             SDN_CONF_DEFAULT_NET;
  SDN_CONF.cfg_id =              0; // TODO
  SDN_CONF.hops =                0;
  SDN_CONF.ft_lifetime =         ((SDN_FT_LIFETIME == 0xFFFF) ? -1 : SDN_FT_LIFETIME);
  SDN_CONF.query_full =          SDN_CONF_QUERY_FULL_PACKET;
  SDN_CONF.query_idx =           SDN_CONF_QUERY_INDEX;
  SDN_CONF.query_len =           SDN_CONF_QUERY_LENGTH;
  sdn_conf_print();
}

/*----------------------------------------------------------------------------*/
void
sdn_conf_print(void){
  /* Node Configuration */
  LOG_DBG(" ...Virt. Network ID: %d\n", SDN_CONF.sdn_net);
  LOG_DBG(" ...Configuration ID: %d\n", SDN_CONF.cfg_id);
  LOG_DBG(" ...Hops from Controller: %d\n", SDN_CONF.hops);
  LOG_DBG(" ...Flowtable TTL: %lu\n", (SDN_CONF.ft_lifetime / CLOCK_SECOND));
  LOG_DBG(" ...Query Full Packet: %d\n", SDN_CONF.query_full);
  LOG_DBG(" ...Query Index: %d\n", SDN_CONF.query_idx);
  LOG_DBG(" ...Query Length: %d\n", SDN_CONF.query_len);
}
/*----------------------------------------------------------------------------*/
/** @} */
