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
$ vagrant up
```

```shell
$ vagrant status

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
$ vagrant ssh nfsserver
```

And in a different window:

```shell
$ vagrant ssh nfsclient1
```

### Exercises

#### Basic networking verification and warm up

* Ping each host from one host

From one of the hosts:
```shell
$ ping 192.168.1.10
$ ping 192.168.1.11
$ ping 192.168.1.12
$ ping 192.168.1.13
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
- ashley:
