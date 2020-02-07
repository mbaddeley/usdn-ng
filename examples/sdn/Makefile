#----------------------------------------------------------------------------#
# Common Makefile Configuration #
#----------------------------------------------------------------------------#
print-%:
	@echo '$*=$($*)'

CONTIKI = ../../..

# https://github.com/contiki-os/contiki/wiki/Reducing-Contiki-OS-firmware-size
CFLAGS += -ffunction-sections
LDFLAGS += -Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__
# Linker Optimization
SMALL=1

# NETSTACK Configuration
MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_ROUTING ?= MAKE_ROUTING_RPL_LITE
MAKE_SDN ?= MAKE_SDN_USDN

# MODULES += os/services/multiflow

# Node statistics (not just for SDN...)
WITH_SDN_STATS ?= 0
ifeq ($(WITH_SDN_STATS),1)
    CFLAGS += -DWITH_SDN_STATS=1 -DSDN_STATS_PERIOD=60
endif

# Server Reply
REPLY ?= 0
ifneq ($(REPLY),)
    CFLAGS += -DSERVER_REPLY=$(REPLY)
endif
