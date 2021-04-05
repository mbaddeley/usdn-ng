#!/usr/bin/env bash

set -xe

# Init contiki submodules
cd contiki-ng
git submodule update --init --recursive
cd ..

# Install i386 binary support on x64 system and required tools
sudo dpkg --add-architecture i386
sudo add-apt-repository universe
sudo apt update
sudo apt install -y --no-install-recommends \
  libc6:i386 libstdc++6:i386 libncurses5:i386 libz1:i386 \
  build-essential doxygen git wget unzip python-serial rlwrap npm \
  default-jdk ant srecord python-pip iputils-tracepath uncrustify \
  mosquitto mosquitto-clients valgrind libcoap-1-0-bin \
  smitools snmp snmp-mibs-downloader \
  python-magic linux-image-extra-virtual openjdk-8-jdk \

sudo apt-get clean
sudo python2 -m pip install intelhex sphinx_rtd_theme sphinx

# Install ARM toolchain
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2
tar -xjf gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2 -C /tmp/
sudo cp -f -r /tmp/gcc-arm-none-eabi-10-2020-q4-major/* /usr/local/
rm -rf /tmp/gcc-arm-none-eabi-* gcc-arm-none-eabi-*-linux.tar.bz2

# Install msp430 toolchain
wget http://simonduq.github.io/resources/mspgcc-4.7.2-compiled.tar.bz2
tar xjf mspgcc*.tar.bz2 -C /tmp/
sudo cp -f -r /tmp/msp430/* /usr/local/
rm -rf /tmp/msp430 mspgcc*.tar.bz2

# Install NXP toolchain (partial, with binaries excluded. Download from nxp.com)
# wget http://simonduq.github.io/resources/ba-elf-gcc-4.7.4-part1.tar.bz2
# wget http://simonduq.github.io/resources/ba-elf-gcc-4.7.4-part2.tar.bz2
# wget http://simonduq.github.io/resources/jn516x-sdk-4163-1416.tar.bz2
# mkdir -p /tmp/jn516x-sdk /tmp/ba-elf-gcc
# tar xjf jn516x-sdk-*.tar.bz2 -C /tmp/jn516x-sdk
# tar xjf ba-elf-gcc-*part1.tar.bz2 -C /tmp/ba-elf-gcc
# tar xjf ba-elf-gcc-*part2.tar.bz2 -C /tmp/ba-elf-gcc
# sudo cp -f -r /tmp/jn516x-sdk /usr/
# sudo cp -f -r /tmp/ba-elf-gcc /usr/
# rm -rf jn516x*.bz2 ba-elf-gcc*.bz2 /tmp/ba-elf-gcc* /tmp/jn516x-sdk*
#
# echo 'export PATH="/usr/ba-elf-gcc/bin:${PATH}"' >> ${HOME}/.bashrc

# Install nRF52 SDK
wget https://developer.nordicsemi.com/nRF5_IoT_SDK/nRF5_IoT_SDK_v0.9.x/nrf5_iot_sdk_3288530.zip
sudo mkdir -p /usr/nrf52-sdk
sudo unzip nrf5_iot_sdk_3288530.zip -d /usr/nrf52-sdk
rm nrf5_iot_sdk_3288530.zip

echo "export NRF52_SDK_ROOT=/usr/nrf52-sdk" >> ${HOME}/.bashrc

sudo usermod -aG dialout vagrant

# Make sure we're using Java 8 for Cooja
sudo update-java-alternatives --set /usr/lib/jvm/java-1.8.0-openjdk-amd64

# Environment variables
echo "export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64" >> ${HOME}/.bashrc
echo "export CONTIKI_NG=${HOME}/contiki-ng" >> ${HOME}/.bashrc
echo "export COOJA=${CONTIKI_NG}/tools/cooja" >> ${HOME}/.bashrc
echo "export PATH=${HOME}:${PATH}" >> ${HOME}/.bashrc
echo "export WORKDIR=${HOME}" >> ${HOME}/.bashrc
source ${HOME}/.bashrc

# Create Cooja shortcut
# echo "#!/bin/bash\nant -Dbasedir=${COOJA} -f ${COOJA}/build.xml run" >> ${HOME}/cooja && chmod +x ${HOME}/cooja

# Docker
# sudo apt update
# sudo apt install -y --no-install-recommends apt-transport-https ca-certificates curl gnupg lsb-release
# curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
# echo \
#   "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
#   $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
# sudo apt update
# sudo apt-get install -y --no-install-recommends docker-ce docker-ce-cli containerd.io

# Get docker using get-docker script
curl -fsSL get.docker.com -o get-docker.sh
sudo sh get-docker.sh

sudo usermod -aG docker vagrant

# Docker image "Contiker" alias
echo 'alias contiker="docker run --privileged --sysctl net.ipv6.conf.all.disable_ipv6=0 --mount type=bind,source=$CONTIKI_NG,destination=/home/user/contiki-ng -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /dev/bus/usb:/dev/bus/usb -ti contiker/contiki-ng"' >> /home/vagrant/.bashrc
source ${HOME}/.bashrc

shutdown -h now
