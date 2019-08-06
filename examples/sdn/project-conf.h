#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* Include controller header */
#ifdef CONTROLLER_CONF_PATH
#include CONTROLLER_CONF_PATH
#endif /* CONTROLLER_CONF_PATH */

#define LOG_CONF_WITH_ANNOTATE              0

/*---------------------------------------------------------------------------*/
/* !!! THERE ARE ALSO SOME CONFIGUARTION OPTIONS IN atom-conf.h WHICH SHOULD BE
   INSPECTED BEFORE TRYING TO RUN THIS APPLICATION !!! */
/*---------------------------------------------------------------------------*/
#define ATOM_CONF_MAX_NODES                 20

/*---------------------------------------------------------------------------*/
/* Reduce uSDN size for SKY */
/*---------------------------------------------------------------------------*/
#define SDN_CONF_FT_MAX_WHITELIST 5
#define SDN_CONF_FT_MAX_MATCHES   5
#define SDN_CONF_FT_MAX_ACTIONS   5
#define SDN_CONF_FT_MAX_ENTRIES   5
#define SDN_CONF_FT_MAX_DATA_MEMB 100

/*---------------------------------------------------------------------------*/
/* SDN Core Configuration */
/*---------------------------------------------------------------------------*/
#if UIP_CONF_IPV6_SDN
/* SDN Stats */
#define SDN_CONF_STATS                      0

/* SDN Packetbuffer Configuration */
#ifdef CONF_NUM_APPS
#define SDN_CONF_PACKETBUF_LEN              CONF_NUM_APPS
#else
#define SDN_CONF_PACKETBUF_LEN              1
#endif
#define SDN_CONF_PACKETBUF_LIFETIME         20      /* seconds. */

/* Allows a callback after packet sent in sicslowpan */
#define SDN_SICSLOWPAN_CALLBACK             0

/* Sets the the first flowtable entry as a default (fast copy) entry */
#define SDN_CONF_DEFAULT_FT_ENTRY           1

/* Options already in makefile */
#ifndef SDN_CONF_FT_LIFETIME
#define SDN_CONF_FT_LIFETIME                0xFFFF  /* seconds. 0xFFFF = inf */
#endif
#ifndef SDN_CONF_CONTROLLER_UPDATE_PERIOD
#define SDN_CONF_CONTROLLER_UPDATE_PERIOD   20      /* seconds */
#endif
#ifndef SDN_CONF_FORCE_UPDATE
#define SDN_CONF_FORCE_UPDATE               1
#endif
#ifndef SDN_CONF_RETRY_AFTER_QUERY
#define SDN_CONF_RETRY_AFTER_QUERY          0       /* N.B. This increases average lat */
#endif

#endif /* UIP_CONF_IPV6_SDN */

/*---------------------------------------------------------------------------*/
/* Memory Optimisations to improve memory usage */
/*---------------------------------------------------------------------------*/
// #define PROCESS_CONF_NO_PROCESS_NAMES 1
#define UIP_CONF_DS6_DEFRT_NBU              1
/* These can be fiddled with depending on what you're trying to do. Note that
   if there are any packet sizes > 117B you'll need the frag on */
#define SICSLOWPAN_CONF_FRAG                0
#define UIP_CONF_TCP                        0

#define UIP_CONF_UDP_CONNS                  2
#define NBR_TABLE_CONF_MAX_NEIGHBORS        4
#define UIP_CONF_MAX_ROUTES                 0  /* No need for node routes */

/*---------------------------------------------------------------------------*/
/* RPL Configuration */
/*---------------------------------------------------------------------------*/
#define RPL_CONF_STATS                              0
#define RPL_CONF_DEFAULT_LIFETIME_UNIT              60 /* seconds */
#define RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME    1

/*---------------------------------------------------------------------------*/
/* Contiki Configuration */
/*---------------------------------------------------------------------------*/
#define UIP_CONF_ROUTER                    1

/*---------------------------------------------------------------------------*/
/* Logging Levels */
/*---------------------------------------------------------------------------*/
#define LOG_CONF_LEVEL_MAIN                LOG_LEVEL_NONE

#endif /* PROJECT_CONF_H_ */
