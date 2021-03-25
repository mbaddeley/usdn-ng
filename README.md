# μSDN-NG - Embedded SDN Stack Low-Power IoT.

## Intro

This repo hosts the source code of μSDN-NG, a Contiki-NG port of the orginal μSDN [here](https://github.com/mbaddeley/usdn) ([paper](https://michaelbaddeley.files.wordpress.com/2019/05/evolving.pdf) | [slides](https://michaelbaddeley.files.wordpress.com/2020/03/netsoft-2018-slides.pdf)).

## Publications

[M. Baddeley, R. Nejabati, G. Oikonomou, M. Sooriyabandara, and D. Simeonidou, “Evolving SDN  for Low-Power IoT Networks,” in 2018 IEEE Conference on Network Softwarization (NetSoft), June 2018.](https://michaelbaddeley.files.wordpress.com/2019/05/evolving.pdf)

[M. Baddeley, R. Nejabati, G. Oikonomou, S. Gormus, M. Sooriyabandara and D. Simeonidou, "Isolating SDN control traffic with layer-2 slicing in 6TiSCH industrial IoT networks," 2017 IEEE Conference on Network Function Virtualization and Software Defined Networks (NFV-SDN), Berlin, 2017, pp. 247-251.](https://michaelbaddeley.files.wordpress.com/2019/05/isolating.pdf)

## About

μSDN is has been developed to provide an open source platform to deliver SDN on 6LoWPAN IEEE 802.15.4-2012 networks. The version here is currently not compatible with TSCH, though we have previously tried it out with our own 6TiSCH implementation for Contiki (check out the NFV-SDN 2017 paper [here](https://michaelbaddeley.files.wordpress.com/2019/05/isolating.pdf)).

Alongside μSDN itself, we provide an embedded SDN controller, *Atom*, as well as a flow generator for testing purposes, *Multiflow*.

Please note, this is an academic exercise and a fairly large codebase, so there are many things in μSDN which may not have been done as cleanly or transparently as I may have liked (though I have tried to clean it up somewhat). I've tried to give a brief overview of all the features and modules here, and you can find the paper and slides within at the top level, but if you find yourself getting lost then I'm happy to answer any questions you may have.

## Virtual Machine Install

If you are unfamiliar with Linux and embedded compilers, I recommend you install uSDN-NG onto a Virtual Machine. This can be done from Windows, Linux, or Mac. If you follow the instructions below, Vagrant will automatically set up a VirtualBox Ubuntu image for you, as well as automatically provisioning all required packages and tools. These instructions are taken from the [Contiki-NG Wiki](https://github.com/contiki-ng/contiki-ng/wiki/Vagrant) on how to set up Vagrant. If you wish to do things like set up USB ports on your image, please refer to that page.

**N.B. This process will take a LONG time. Although the VirtualBox VM will automatically pop up, DO NOT do anything in that VM window during the install process.**

### Instructions

* Install a virtualization platform such as [VirtualBox](https://www.virtualbox.org)
* Install Vagrant from [www.vagrantup.com/downloads](https://www.vagrantup.com/downloads.html)
* Open a terminal (e.g. [git bash](https://gitforwindows.org/) in Windows).
* Get the uSDN-NG repository
```bash
$ sudo apt-get install git
$ git clone https://github.com/mbaddeley/usdn-ng.git
```
* Initialize Vagrant image
```bash
$ cd contiki-ng/tools/vagrant
$ vagrant up
```

* Log in to the Vagrant image
```bash
$ vagrant ssh
```
* In case of a Windows host, you may have to convert line endings of the bootstrap.sh script. From the Vagrant shell:
```bash
$ sudo apt update
$ sudo apt install dos2unix
$ dos2unix contiki-ng/tools/vagrant/bootstrap.sh
```
* Install Contiki-NG toolchain. From the Vagrant shell:
```bash
$ sudo ./contiki-ng/tools/vagrant/bootstrap.sh
```
* Exit the SSH session
```bash
$ exit
```

* Restart the vagrant image.
```bash
$ vagrant reload
```

* Log in to the Vagrant image again and install the VM GUI.
```bash
$ vagrant ssh
$ sudo ./contiki-ng/tools/vagrant/bootstrap-vbox-with-x.sh
$ exit
$ vagrant reload
```

This will install VirtualBox guest additions, X and the Xfce desktop environment.

* Login using username 'vagrant' and password 'vagrant'
* Run `sudo startx`.
* This will start the desktop environment. You should now see the VM's desktop environment. You can start cooja by opening a terminal and typing the following.
```bash
$ contiker cooja
```

If this is the first time opening docker, it will need to pull the image from ther server. This may take a few minutes. Once it's finished you should now see Cooja up and running!

## Native Linux Install

Alternatively, you can install all the native toolchains on a Linux/Mac. This will make things like compilation and development MUCH faster. If you know your way around Linux, and you want a proper development environment, then this is the way to go. Instructions on how to do this can be found on the Contiki-NG Wiki:

- [Native toolchain installation (Linux)](https://github.com/contiki-ng/contiki-ng/wiki/Toolchain-installation-on-Linux)
- [Native toolchain installation (macOS)](https://github.com/contiki-ng/contiki-ng/wiki/Toolchain-installation-on-macOS)

I'd recommend installing the following, which will allow you to test Z1 and Cooja motes (in cooja), and compile nRF52840 motes using real-world hardware...

- [Docker (for Cooja)](https://github.com/contiki-ng/contiki-ng/wiki/Docker)
- [Contiki-NG development tools](https://github.com/contiki-ng/contiki-ng/wiki/Toolchain-installation-on-Linux#install-development-tools-for-contiki-ng)
- [msp430-gcc compiler (for Z1 motes)](https://github.com/contiki-ng/contiki-ng/wiki/Toolchain-installation-on-Linux#install-msp430-compiler)
- [nRF52840 support](https://github.com/contiki-ng/contiki-ng/wiki/Platform-nrf52840)

## How to Compile

The available targets I've tested in Cooja for uSDN-NG are *cooja* and *z1*. **NB: The *cooja* target can only be compiled within Cooja itself. If you try to compile it through the terminal you'll get an error.**

- TARGET=cooja
- TARGET=z1

You can make the controller and node by manually going into the respective directories...

```
  cd usdn/examples/sdn/controller
  make clean TARGET=<your-target> && make TARGET=<your-target> <add-other-your-makeargs-here>
  cd ../node
  make clean TARGET=<your-target> && make TARGET=<your-target> <add-other-your-makeargs-here>
```

...but there is a handy compile script in there that can be used to compile both the controller and node at once for you:

```
  cd usdn/examples/sdn
  ./compile.sh TARGET=<your-target> <add-other-your-makeargs-here>
```

The following make arguements are available:

uSDN Make Args:
- NSUFREQ - Frequency of node state updates to the controller (seconds)
- FTLIFETIME - Lifetime of flowtable entries (seconds)
- FTREFRESH - Refresh flowtable entries on a match (0/1)
- FORCENSU - Immediately send a NSU to the controller on join (0/1)
- LOG_LEVEL_SDN - Set the uSDN log level (0 - 5)
- LOG_LEVEL_ATOM - Set the Atom controller log level (0 - 5)

Multiflow Make Args:
- MULTIFLOW - Turn on multiflow (0/1)
- NUM_APPS - Number of flows (N)
- FLOWIDS - Id for each flow ([0,1,2...])
- TXNODES - Transmission nodes ([18,12...] 0 is ALL)
- RXNODES - Receiving nodes ([1,2...])
- DELAY   - Delay each transmission (seconds)
- BRMIN   - Minimum bitrate (seconds)
- BRMAX   - Maximum bitrate (seconds)

e.g.:

```
./compile.sh TARGET=z1 MULTIFLOW=1 NUM_APPS=1 FLOWIDS=1 TXNODES=8 RXNODES=10 DELAY=0 BRMIN=5 BRMAX=5 NSUFREQ=600 FTLIFETIME=300 FTREFRESH=1 FORCENSU=1 LOG_LEVEL_SDN=LOG_LEVEL_DBG LOG_LEVEL_ATOM=LOG_LEVEL_DBG
```

## Where is everything?

- Core: */core/net/sdn/*
- Stack: */core/net/sdn/usdn/*
- Atom: */apps/atom/*
- Multiflow: */apps/multiflow/*

### Overview of optimization features
- Fragmentation elimination.
- Configurable controller update frequency.
- Configurable flowtable lifetimes.
- Flowtable matches on packet index + byte array.
- Source routed control packets.
- Throttling of repeated control packets.
- Refreshing of regularly used flowtable entries.
- SDN node configuration.
- Control packet buffer.

### Core

- sdn.c

  Main SDN process.

- sdn-cd.c

  Controller discovery and join services.

- sdn-conf.c

  SDN node configuration.

- sdn-ft.c

  SDN flowtable(s).

- sdn-ext-header.c

  Extension headers for source routing.

- sdn-packetbuf.c

  Buffer to allow storing of packets whilst nodes query the controller.

- sdn-timer.h

  Timer configuration and macros.

- sdn-stats.c

  SDN statistics. This also replaces the powertrace application from Contiki.

### Stack
- usdn-driver.c

  The driver interfaces with the flowtable to provide a basic API to allow specific functions, such as forwarding packets based on source/destination, aggregating packets, dropping packets, etc.

- usdn-engine.c

  The engine handles incoming and outgoing SDN messages, using the API provided by the driver.

- usdn-adapter.c

  The adapter provides a connection interface to the controller for a specific protocol. Currently this is for the μSDN protocol.

### ATOM Controller
- atom.c

  Main process for atom.

- atom-buffer.c

  Buffer for incoming and outgoing packets.

- atom-net.c

  Keeps tracks of and abstracts the network state for the controller. My original idea for this was that the controller should be able to configure the network so that nodes update the view with the required metric, rather than all metrics, in order to keep the memory requirements down.

- atom-sb-xx.c

  Atom allows you to create multiple southbound connectors. Currently we have a μSDN and RPL connector implemented.

- atom-app-net-xx.c

  Network applications.

- atom-app-route-xx.c

  Routing applications.

- atom-app-join-xx.c

  Join/association applications.

### Still to be Implemented...

- Perform multiple flowtable entries on packets

### Known Issues

- Shortest Path Routing doesn't work well with larger network sizes (30+), use RPL-NS routing in this case! This is because it takes too long to compute all the possible paths on the central atom controller, and there are no checks to queue additional requests while it's computing.
- Lots ;) Just ask if you have problems and I'll try to help as best I can.


---
[usdn_ng_v1.1] - 25/03/21
---
- Fixed vagrant image to allow easy install

---
[usdn_ng_v1.0] - 02/03/21
---
- Initial port to NG. Working on cooja and z1 motes (in cooja).

---
[usdn_v1.2] - 19/03/19
---
- Added compilation script so you can compile in one command: `./compile.sh ADD_MAKEFILE_ARGS_HERE*`
- Added a redirect example: *usdn-redirect.csc*
- Modified *uip6.c* and *usdn-driver.c* so that RPL source routing packets are no longer automatically ignored by the SDN.
- Changed Atom controller to use shortest path routing by default rather than RPL-NS routing!

---
[usdn_v1.1] - 17/07/18
---
- Turned on default FT lifetimes.
- Fixed issue where SDN was turned off in the Makefile (oops!).
- Added hopcount to Atom ingress messages.
- Added the NetSoft 2018 paper and slides.
- Added Makefile.target so you don't need to type in target each time.
- Fixes for automatic simulation through ContikiPy.

---
[usdn_v1.0] - 28/06/18
---
- Initial commit.
