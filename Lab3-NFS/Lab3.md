<!---
   Copyright 2014 Portland State University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
--->

Lab 3: NFS
===================


In this lab you will be playing with NFS, the Network File System. This is kindof a hardcore service. For Realz. Dis aint the Web Dump, homie.


Section 1: Networking Basics
----------------------------


For this lab we will have a multi-vm setup inside vagrant. All three vms will be running Centos Linux. Each will have its own IP Address.



Take a quick look at your vagrantfile:

This file can be your reference for which machines have which names and ip addresses.


### Vagrant up

```shell
vagrant up
```

```shell
vagrant status

Current machine states:

nfsserver                 running (virtualbox)
nfsclient1                running (virtualbox)
nfsclient2                running (virtualbox)
nfsclient3                running (virtualbox)

This environment represents multiple VMs. The VMs are all listed
above with their current state. For more information about a specific
VM, run `vagrant status NAME`.
```

### SSH into multiple hosts

Open up several terminal windows, change directory on all of them into the Lab3 directory.

```shell
vagrant ssh nfsserver
```

And in a different window:

```shell
vagrant ssh nfsclient1
```

### Exercises

#### Basic networking verification and warm up

* Ping each host from one host

From one of the hosts:
```shell
ping 192.168.1.10
ping 192.168.1.11
ping 192.168.1.12
ping 192.168.1.13
```

* View the routing table for your virtual machines

```
$ netstat -rn
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
192.168.1.0     0.0.0.0         255.255.255.0   U         0 0          0 eth1
10.0.2.0        0.0.0.0         255.255.255.0   U         0 0          0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U         0 0          0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U         0 0          0 eth1
0.0.0.0         10.0.2.2        0.0.0.0         UG        0 0          0 eth0
````
Destination: The subnet for ip adress ranges, the final destination. It the case of the first one, the addresses 192.168.1.0-255  
Gateway: The next level up for routing. A 0.0.0.0 gateway means "unspecified", and any ipaddress going through the netmask would be let through this gateway  
Genmask: The netmask for the destination host  
Flags: Flag U means that the interface is up, the G flag indicates a route to host via a gateway  
Iface: Interface to which packets are sent to  

* Set up three users on nfsserver and nfsclient1, nfsclient2, and nfsclient3
- ashley: uid=2313
- bob:    uid=2121
- mike:   uid of your choice

```shell
sudo useradd -u 2313 ashley
sudo useradd -u 2121 bob
sudo useradd -u $UID mike
```
You can see some information about the users with ``id <username>``

* Set passwords for the users (including root)

```shell
$ sudo passwd <username>
```

* Create a file foo.txt with plain text in it, and copy it using scp from one host to another using your created users

As each of the users you just created (on any host), create a file foo.txt and send it to different hosts (have destinations be different for each file)
```shell
sudo su <username> (not just for becoming root 0.0)
echo "I AM A FILE!" > foo.txt
scp <file to send> <username>@<destination ip>:<destination path>

#Leaving out the username will use the user logged in as by default
#ip address can be replaced with hostname if set
#Leaving out the destination path will use /home/$USER by default
```
Verify that the file was sent successfully by running ``ls /home/<username>`` on the destination host.

Run ``exit`` to go back to root user

* Set up a hosts file on nfsserver to map each ip address in your network to the common name given in the vagrant file.

Add the following lines to /etc/hosts on each server, leaving out the line of the server you are currently on
```shell
#EXAMPLE: on nfsclient1 leave out the line with nfsclient1 and its ip addr

192.168.1.10 nfsserver
192.168.1.11 nfsclient1
192.168.1.12 nfsclient2
192.168.1.13 nfsclient3
```
Try to ping one of the other hosts by hostname. You should get the same output you would if you pinged by ip addresses.

Section 2: NFS: The Network File System
---------------------------------------

NFS, above all other *nix services, is the core of what the CAT does. Since the dawn of multisystem networking janaka and his minions have been carving space off of the big iron and exporting it to the puny clients.

On a conceptual level, NFS is one computer letting another computer use its storage space. It is also one-to-many, meaning that one NFS server can export a filesystem and many client computers can mount and utilize this file system.


### Setting up the NFS packages

* Install the NFS packages on each of the hosts

```shell
sudo yum install nfs-utils
```

* Start the daemon on each of the hosts

```shell
sudo service nfs start
```

* Get NFS service to start upon booting

We want our new service to start when the machine is booted. We manage this with "chkconfig".  
You can see the the levels a service is set to run at with ``chkconfig --list nfs``.  
Turn on levels 3,4,5 with ``chkconfig nfs on`` (3: runs the service under a multiuser mode, 4: unused, 5: runs on a xsession)

* Create directories /data/share1 through /data/share4 on nfsserver

On nfsserver run:
```shell
sudo mkdir -p /data/share{1,2,3,4}
```
Create a group for users that should have access to the shares on each of the hosts
```shell
sudo groupadd -g 1050 datashare
#NOTE: group ids 999 and below are usually reserved for system accounts
```
Add the users you created (and root) to the group you just created on each of the hosts
```shell
usermod -a -G datashare <username>
```
Check the which groups the user is a member of
```shell
groups <username>
```

Give group ownership of the data shares to group datashare (on nfsserver)
```shell
chgrp datashare /data/share*
```
Check the ownership of the shares (on nfsserver)
```shell
ls -l /data/
```
Change the perms on the directories so anybody in group datashare can create and edit files on them (on nfsserver)
```shell
chmod 775 /data/share*
```

### /etc/exports

The NFS server is configured by a file called /etc/exports. This file tells the nfs server which directories to export and which machines to export them to. NFS, on its own, does not do per-user authentication. Rather any machine in the access list can mount the remote filesystem with the options given in /etc/exports.

You can read about /etc/exports at http://www.centos.org/docs/5/html/Deployment_Guide-en-US/s1-nfs-server-config-exports.html

### Setting up NFS configs

* Open nfs and tcp/udp ports on nfsserver for the ip addresses of the clients

We need to open ports 2049 (default port for nfs) and 111 (default port for portmapper) to allow tcp/udp protocols on our subnet.

Run these commands on nfsserver
```shell
sudo iptables -I INPUT -p tcp -s 192.168.1.0/24 -m state --state NEW,RELATED,ESTABLISHED --dport 2049 -j ACCEPT
sudo iptables -I INPUT -p udp -s 192.168.1.0/24 -m state --state NEW,RELATED,ESTABLISHED --dport 2049 -j ACCEPT
sudo iptables -I INPUT -p tcp -s 192.168.1.0/24 -m state --state NEW,RELATED,ESTABLISHED --dport 111 -j ACCEPT
sudo iptables -I INPUT -p udp -s 192.168.1.0/24 -m state --state NEW,RELATED,ESTABLISHED --dport 111 -j ACCEPT
```
This will add some rules to iptables that will allow nfs request packets to be accepted on nfsserver.  
``-I INPUT`` specifies what will be done with the packets. Option "INPUT" means this host will be recieving packets.  
``-m state --state NEW,RELATED,ESTABLISHED`` defines the state of the connection that the rule should obey.  
-NEW : The connection has not been seen before  
-RELATED : The connection is NEW, but is related to a connection already permitted  
-ESTABLISHED : The connection has been made before  
``-j ACCEPT`` tells the host what to do with the packet. "ACCEPT" tells it to accept the packet and stop reading the rule.

Save the changes and restart the service
```shell
sudo service iptables save
sudo service iptables restart
```
Check whether the settings were saved with ``iptables -L``

* Set the directory /data/share1 to be shared with standard permissions to all hosts

Add the following lines to /etc/exports on nfsserver
```shell
/data/share1 *(rw)
```
Execute ``exportfs -rv`` on nfsserver

* Mount the filesystem on nfsclient1

```shell
sudo mount -t nfs nfsserver:/data/share1 /mnt
```
Check if the FS has been mounted
```shell
mount | grep nfsserver
```
You should get some information on about the mounted FS.

su into another user and try to make a file on the mounted filesystem
```shell
touch /mnt/testfile
```
Now mount share1 on another nfs_client. Can you see testfile there?
su into the same user and edit the file. Go back to nfsclient1 and look at the file. Can you see the changes?

Dont forget to unmount the filesystems when you are done with them
```shell
umount /mnt
```

NOTE: If you try to create a file as root on a share with root_squash on (on by default), it will be created under user "nobody".

* Share /data/share2 to only nfsclient2

Add this to /etc/exports on nfsserver
```shell
/data/share2 nfsclient2(rw)
```
Execute ``exportfs -rv`` on nfsserver
Try mounting share2 on a nfsclient other than client2. Does it work? (it shouldnt)
Now try mounting share2 on nfsclient2. Does this work? (no error messages)

Check the FS with ``df``

Unmount the FS when you are done with it.
* Share /data/share3 to only clients in the 192.168.1.0/24 subnets

Add this to /etc/exports on nfsserver
```shell
/data/share3 192.168.1.0/24(rw)
```
Execute ``exportfs -rv`` on nfsserver.  
Now try mounting share3 on all of the nfsclients . Does this work? Check the mount using either of the previously specified methods

Unmount share3 from all of the clients.

* Share /data/share4 to all clients in your subnet but no other ip addresses, but turn off root squashing
Add this to /etc/exports on nfsserver
```shell
/data/share4 192.168.1.0/24(rw,no_root_squash)
```
Run ``exportfs -rv`` on nfsserver
Mount share4 on any nfs_client. Check if it mounted. Trying making a file as root on the share. Run ``ls -l``. Who is the owner of the file (with root squashing off, it should be "root")

### Exercises

* On the clients, verify that you can create a file on an NFS share with one user, and pull it off with another user on another system
* On one client, create a large file and open it with vim. On another client, rm the file. What happens?
* ``showmount -e nfsserver`` on any of the clients should show what is available to mount from the nfsserver, but an error occurs. How can we fix this? (Hint: iptables...)

### Have NFS share mounted on boot

So we have a few shares setup, but we have to mount them every time the host is rebooted. If you a using NFS for user homedirs (for example), you want that FS to be mounted for you.

* Modify /etc/fstab

fstab is the configuration file that manages filesystems on Unix operatings systems. Upon booting, the machine mounts different FS based on this file.

Lets add share1 to be mounted on startup. We shouldnt mount this on /mnt because it should be left for more dynamic mounts.

Create a directory any of the nfsclient1 for /data/share1 to me mounted to
```shell
sudo mkdir /datashare
```
Now lets add this NFS share to be mounted on startup.
```shell
sudo vim /etc/fstab

#Though the file does not have headers for the columns, this is what each column specifies
# Device                   mountpoint   fs-type   options       dump  fsckord

#add this line to the bottom
 nfsserver:/data/share1    /datashare     nfs    rw,hard,intr    0      0
```
Run ``sudo shutdown -r now``

Wait a couple of minutes, then run ``vagrant ssh nfsclient($NUM)``
Run ``ls /datashare``. Can you see "file1"? (You should)

### Autofs

You can set filesystems to be automatically mounted when it is needed. This could be useful when you have many mountable NFS shares, but dont always need all of them. You will have access to share you need without having to manually mount each share and have not have all of the shares mounted all of the time.

* Configure a client to use autofs with share3

Install the autofs package on nfsclient2
```
sudo yum install autofs
```
The primary configuration file for autofs is /etc/auto.master . We are going to create a automount point for share3

Add the following line to /etc/auto.master (on nfsclient2) just under the line with ``/misc   /etc/auto.misc``
```
/share  /etc/auto.share
```
The first column specifies where the base mountpoint is, the second column points to the file where the NFS share is sourced at.

Now create a file /etc/auto.share as root on nfsclient2 and put the following line in it:
```
share3 -fstype=nfs nfsserver:/data/share3 
```
Column 1 specifies the directory that the FS will be mounted to (following the base mountpoint).  
You should be able to recognize what columns 2 and 3 are all about by now.  
More information on configurations for these files can be found at http://www.centos.org/docs/5/html/5.2/Deployment_Guide/s2-nfs-config-autofs.html

Restart autofs
```
sudo service autofs restart
```

* Check to see if automount works
- Run ``ls /``. You will see that the directory /share has been created for you  
- Run ``ls /share``. Can you see share3? (you shouldnt, it has not been mounted yet)  
- Use either ``df`` or ``mount | grep nfs`` to check the mounts on the host  
- Automount creates the directory and mounts the NFS share right when the user attempts to open that file, but before that it will not be displayed or shown as mounted. To trigger automount, simply ``cd`` into the directory you know should be mounted. Here in the CAT we like to refer to that as "the leap of faith"  
- Check your current directory with ``pwd``
- Check the mount with either ``df`` or ``mount | grep nfs``

### Excercises
* Reboot nfsclient2 and run ``ls /share`` once its back up. What do you see?
* Setup share4 to automount, but timeout if the share is not accessed for 5 minutes


