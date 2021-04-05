# μSDN VM Install 

## Prerequisites

* Install a virtualization platform such as [VirtualBox](https://www.virtualbox.org)
* Install Vagrant from [www.vagrantup.com/downloads](https://www.vagrantup.com/downloads.html)
* Make sure git is installed (e.g. `sudo apt install git` in Linux or [git bash](https://gitforwindows.org/) in Windows)

## Installation 

Open a terminal and get the uSDN-NG repository:

```bash
$ git clone https://github.com/mbaddeley/usdn-ng.git
```

Initialize Vagrant image:

```bash
$ cd usdn-ng/tools/vagrant
$ vagrant up
```

## Usage

After the initialization finished, the VM will automatically shutdown. Please restart it again manually using `vagrant up`.

The first time you open the VM, you need to login using the username 'vagrant' and password 'vagrant'. And if there is a pop-up to ask you to do the release upgrade, DO NOT do that! The script haven't test on a higher ubuntu version.

You should now see the VM's desktop environment. You can start Cooja by opening a terminal and typing the following:

```bash
$ contiker
```

If this is the first time opening docker, it will need to pull the image from their server. This may take a few minutes. Once it's finished you should now see Cooja up and running!

Remember, you should always use the `vagrant up` to open the VM; open it in the VirtualBox will fail to load the sync folder.
