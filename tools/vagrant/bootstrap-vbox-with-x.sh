#!/usr/bin/env bash

sudo apt update

# Install XFCE4 and VirtualBox Guest Additions
sudo apt install -y xfce4 \
  virtualbox-guest-dkms virtualbox-guest-utils virtualbox-guest-x11

# Permit any user to fire up the desktop environment
sudo sed -i 's/allowed_users=.*$/allowed_users=anybody/' /etc/X11/Xwrapper.config

# disable screensaver
sudo apt-get remove -y light-locker

# automatic login
cat << EOF | tee -a /etc/lightdm/lightdm.conf.d/10-lightdm.conf
[SeatDefaults]
autologin-user=vagrant
autologin-user-timeout=0
user-session=Xubuntu
EOF

sudo VBoxClient --clipboard
sudo VBoxClient --draganddrop
sudo VBoxClient --display
sudo VBoxClient --checkhostversion
sudo VBoxClient --seamless
