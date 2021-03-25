#!/usr/bin/env bash

set -xe

sudo apt-get update -y
sudo apt-get upgrade -y

# Install XFCE4 and VirtualBox Guest Additions
sudo apt-get install -y  --no-install-recommends xubuntu-desktop virtualbox-guest-dkms virtualbox-guest-utils virtualbox-guest-x11

# Disable Screensaver
sudo apt-get remove -y light-locker

# Automatic Login
cat << EOF | tee -a /etc/lightdm/lightdm.conf.d/10-lightdm.conf
[SeatDefaults]
autologin-user=vagrant
autologin-user-timeout=0
user-session=Xubuntu
EOF
