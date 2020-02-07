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
 *         uSDN Core: Configuration file to allow setting the driver, engine
 *                    and southbound interface implementations.
 * \author
 *         Michael Baddeley <m.baddeley@bristol.ac.uk>
 */


#ifndef SDN_STACK_H_
#define SDN_STACK_H_

/* SDN Controller Implementations */
#define SDN_CONTROLLER_NONE               0
#define SDN_CONTROLLER_ATOM               1

/* SDN Connection Adapter Implementations */
#define SDN_ADAPTER_NONE                  0
#define SDN_ADAPTER_USDN                  1
#define SDN_ADAPTER_NULL                  2

/* SDN Engine Implementations */
#define SDN_ENGINE_NONE                   0
#define SDN_ENGINE_USDN                   1
#define SDN_ENGINE_NULL                   2

/* SDN Driver Implementations */
#define SDN_DRIVER_NONE                   0
#define SDN_DRIVER_USDN                   1
#define SDN_DRIVER_NULL                   2

#endif /* SDN_STACK_H_ */
