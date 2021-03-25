# μSDN VM Install 

* Install a virtualization platform such as [VirtualBox](https://www.virtualbox.org)
* Install Vagrant from [www.vagrantup.com/downloads](https://www.vagrantup.com/downloads.html)
* Make sure git is installed, then open a terminal (e.g. `sudo apt install git` in Linux or [git bash](https://gitforwindows.org/) in Windows).
* Get the uSDN-NG repository
```bash
$ git clone https://github.com/mbaddeley/usdn-ng.git
```
* Initialize Vagrant image
```bash
$ cd contiki-ng/tools/vagrant
$ vagrant up
```
* After the initialization finished, the VM will automatically restart.
* The first time you open the VM, you need to login using the username 'vagrant' and password 'vagrant'. And if there is a pop-up to ask you to do the release upgrade, DO NOT do that! The script haven't test on a higher ubuntu version.
* You should now see the VM's desktop environment. You can start Cooja by opening a terminal and typing the following.
```bash
$ contiker cooja
```

If this is the first time opening docker, it will need to pull the image from their server. This may take a few minutes. Once it's finished you should now see Cooja up and running!
