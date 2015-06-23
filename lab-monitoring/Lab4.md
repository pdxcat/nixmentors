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

Lab 4: Monitoring
=================

In this lab you will be learning about monitoring services and machines.

Setup
-----

Start the VMs for the lab, ensure all four are running, and then ssh into 
the syslog server and the other machines. Like the last lab, you may find
it convenient to have several windows open.

```bash
$ vagrant up
$ vagrant status
$ vagrant ssh syslogserver
$ vagrant ssh syslogclient1
etc..
```

Section 1: Syslog
-----------------

Syslog is used by services to log errors and other information.

Install rsyslog, a leading syslog implementation, on the 
syslog clients and server, but not the nagios system:

We are using yum because this is a Red Hat distro of linux.

```bash
sudo yum install rsyslog
```
Consider going "sudo -i" at this point.

Modify the rsyslog configuration file: /etc/rsyslog.conf

On the server, uncomment these lines.
The first line loads the UDP server module.
The second makes the UDP server start and listen on port 514.

```bash
 #$ModLoad imudp
 #$UDPServerRun 514
```

On the client side we want to set some message filter rules. These rules
have the following format:

facility.level  destination

You can see examples of this in the rsyslog.conf file where the commented
lines describe who gets what messages. In this case want the server to receive all the 
log events of the client.
  
On the clients, at the bottom, add these lines:
```bash
*.* @syslogserver
```

Vagrant networking has specified some IPs for us to use that we saw in the
last lab. You can see this by examining the Vagrantfile; also you can run
netstat -rn to get a view of how networking is configured. Generally, unless
you alter some config files, we can expect networking to be start at
192.168.1.10, which should be the IP of the server.

Add the server to the /etc/hosts file on the clients:
```bash
192.168.1.10 syslogserver
```

Allow traffic in the firewalling rules for the server at /etc/sysconfig/iptables
Recall how we started the UDP server module on port 514...
Add the following line before "-A INPUT -j REJECT":
```bash
-A INPUT -p udp --dport 514 -j ACCEPT
```

Restore (like reload) from the configuration file we just edited:
```bash
sudo iptables-restore /etc/sysconfig/iptables
```

Restart the rsyslog service on each of the three systems:
```bash
sudo service rsyslog restart
```

Test it out. Send a message from one of the clients ...

While sending test messages to syslog from the clients:
```bash
logger test
```
and tail the message log to see it.

Watch the logs on the server:
```bash
tail -F /var/log/messages
```

You should see your message added to the log file on the syslog server.
If you do not, perhaps you made a typo in editing the /etc/hosts on the
clients, editing the iptables or forgot to restart something along the 
way. If you edit any of the .conf files, be sure to restart the related
service and rsyslog.

Section 2: Nagios
-------------------

Nagios is a popular open source monitoring tool designed to let system administrators
know about problems in their infrastructure. Kinds of things monitored include:
whether an application is running or not, network problems (we have all seen Nagios
complain in robots), security issues and even planning for events. In this lab, you 
will install, configure, add hosts, add services, and set up nrpe.

###Section 2.1: Install and Configure Nagios3

First, ssh into the nagios3 vm. Update, but DO NOT upgrade at this time. Install nagios3 
using apt-get (not yum because Nagios is running on a Ubuntu VM). You will 
be prompted for a how you want nagios to mail you and the default user. Select "local only" 
then type in 'vagrant@localhost'. You will also be prompted for a web administration 
password you can create your own and remember it or simply type "password".

```bash
vagrant ssh nagios3
sudo apt-get update
sudo apt-get install nagios3
```

After, nagios3 is installed head over to "http://localhost:8080/nagios3" you will be prompted by a username and password.
The username is 'nagiosadmin' and password is whatever you entered when you were prompted during setup.

If you are on a laptop or sshing into a machine on which the VM is running you should use
the name of the machine not localhost.

Now, have some fun checking out the site, click on some links, ask questions about what something means if its
confusing. Specifically checkout the Map tab, its gonna look really cool the more hosts you add especially on
'circular'. Its pretty boring right now, but thats why we are gonna add some hosts and services.

###Section 2.2: Adding Hosts and Services

Now its time to head down the deep rabbit hole of configs. Fire up an extra tab and head over to:
  http://www.the-tech-tutorial.com/wp-content/uploads/2011/07/nagios-config.png
This is just a guideline, the nagios.cfg file can be maintained as one long file containing all
the hosts, hostgroups, services, commands, contacts, contactgroups, timeperiods, and more. However,
splitting them up can make each host, command, service, contact, be there own file, making it simple to add, edit, and delete
each item, especially when using a configuration tool like Puppet.

So before we add a host, command, service, and contact. cd into the /etc/nagios3/conf.d 
and make some folders.

```bash
cd /etc/nagios3/conf.d
mkdir hosts commands services contacts
```
#### Section 2.2.1: Adding Hosts

Time to add a host for Nagios to monitor. Specifically, projects.cecs.pdx.edu:
Copy the following files from /vagrant/nagios into /etc/nagios3/ then change
the permissions and reload nagios3.

```bash
cp /vagrant/nagios/hosts/projects.cfg hosts/
chmod 644 hosts/projects.cfg
service nagios3 reload
```
Now go back to the Nagios web app and click on hosts. projects.cecs.pdx.edu should be listed as a host in addition to localhost.

We have our first host being monitored! Now its time to monitor a service.

Let start with dns. Copy the config file for the dnsservers into the hosts directory. Notice this time we created a host configuration for the CAT DNS servers and the Catacombs DNS server.

```bash
cp /vagrant/nagios/hosts/dnsservers.cfg hosts/
chmod 644 hosts/dnsservers.cfg
service nagios3 reload
```
#### Section 2.2.2: Adding Services

Now that we have our dns hosts defined we can add a service check

```bash
cp /vagrant/nagios/services/dns.cfg services/dns.cfg
service nagios3 reload
```
When you do this own your own at some point, how you will know what to put in the 
Nagios3 configuration directory is based on what Nagios documentation says (of course)
but also on what you are trying to monitor.  In the Nagios web app click on services to 
see your pending DNS check. If you do not want to wait to see it turn green, click on 
the DNS link and then click "Re-schedule the next check of this service" and then 
click commit.

Once it turns green in the services tab you should see 


    DNS OK: 0.016 seconds response time. www.google.com returns 173.194.33.16,173.194.33.17,173.194.33.18,173.194.33.19,173.194.33.20

These Nagios commands that are used by the web interface are usually just bash or perl 
scripts. You can easily write your own scripts and have Nagios run them for you. In this
case the one being used is the check_dns plugin. Before they can be used Nagios needs to 
have a command (like check_dns) defined.  Lucky for you Debian ships these configuration 
files for you. You can look at the DNS one by opening the file shown below.

```bash
cat /etc/nagios-plugins/config/dns.cfg
```

Now create your own service check for canhazdns. This service check is used by the web
interface.  Remember we have already defined the host so all you need to do is modify 
services/dns.cfg.  You will likely need to apt-get install vim prior to editing that file.


By the way, you can reproduce what Nagios is doing straight from the command line by 
calling the scripts directly, in this case a script called check_dns from the nagios plugins library.

```bash
$ /usr/lib/nagios/plugins/check_dns -H google.com -s newton.cat.pdx.edu
```

###Section 2.3: NRPE

So far we have monitored hosts remotely from the Nagios server. Now we want to run a local command on a target client. SSH has some extra overhead so most Nagios administrators prefer using NRPE or the Nagios Remote Plugin Executor.

NRPE works by running a client on the target host and waiting for connections from the Nagios server. Once a connection is made the Nagios server instructs the client to run a predefined command on the client and 
report back the status. An example of this would be check the space left on a disk.


On the Nagios server

```bash
apt-get install nagios-nrpe-plugin
```

On the syslogserver

```bash
yum install nrpe
```

Configure allowed_hosts in the NRPE configuration file.

```
# edit /etc/nagios/nrpe.cfg to allow the ip of your nagios server
allowed_hosts=127.0.0.1,192.168.1.13
```

Take a look at the NRPE commands already defined by default.

```bash
cat /etc/nagios/nrpe.cfg | grep check_
```

If you want to add custom ones you will want to add them to /etc/nrpe.d/

Now start the NRPE service

```bash
service nrpe start
```

Now lets test that the that the server can connect

On the Nagios server

```bash
/usr/lib/nagios/plugins/check_nrpe -H 192.168.1.10 -c check_load
Connection refused or timed out
```

Welp we forgot to do something. Can you guess what?

Hint: What does nmap tell you about the host?

```bash
$ apt-get install nmap
$ nmap 192.168.1.10

Starting Nmap 5.21 ( http://nmap.org ) at 2014-04-30 17:56 UTC
Nmap scan report for 192.168.1.10
Host is up (0.00062s latency).
Not shown: 999 filtered ports
PORT   STATE SERVICE
22/tcp open  ssh
MAC Address: 08:00:27:16:FE:61 (Cadmus Computer Systems)
```

Create an Iptables rule for NRPE on the SyslogServer. Rather than modify the iptables
file manually like in the first part, we can use a one-liner to add in what we want.

```bash
iptables -I INPUT -p tcp -m tcp --dport 5666 -j ACCEPT
```

Run NMAP again on the Nagios Server

```bash
$ nmap 192.168.1.10

Starting Nmap 5.21 ( http://nmap.org ) at 2014-04-30 18:01 UTC
Nmap scan report for 192.168.1.10
Host is up (0.00057s latency).
Not shown: 998 filtered ports
PORT     STATE SERVICE
22/tcp   open  ssh
5666/tcp open  nrpe
MAC Address: 08:00:27:16:FE:61 (Cadmus Computer Systems)

Nmap done: 1 IP address (1 host up) scanned in 5.32 seconds
```

This time port 5666 is open and connections from the Nagios Server will work.

Let run the check_nrpe plugin again on the nagios vm.

```bash
/usr/lib/nagios/plugins/check_nrpe -H 192.168.1.10 -c check_load
NRPE: Unable to read output
```

Gah another issue! A quick google search will suggest that your plugins are not installed 
on the client. Lets check the syslogserver for the check_load plugin.

```bash
ls /usr/lib64/nagios/plugins/check_load
ls: cannot access /usr/lib64/nagios/plugins/check_load: No such file or directory
```

Alas it is not there; install the package on the syslogserver.

```bash
yum install nagios-plugins-load
```

Note: RHEL has a package for every plugin, Debian bundles them in one package.

Try our check_nrpe command again on the nagios vm:

```bash
/usr/lib/nagios/plugins/check_nrpe -H 192.168.1.10 -c check_load 
OK - load average: 0.00, 0.00, 0.00|load1=0.000;15.000;30.000;0; load5=0.000;10.000;25.000;0; load15=0.000;5.000;20.000;0; 
```

Success!

If you want to try out the other defined NRPE commands, make sure to install thier packages on the client.

Ok, now that we know that NRPE works manually its time to configure it in Nagios. You should be familiar with the host and service file by now but you should take a look at the commands/load.cfg file.

```bash
cp /vagrant/nagios/commands/load.cfg commands/
chmod 644 commands/load.cfg
cp /vagrant/nagios/hosts/syslogserver.cfg hosts/
chmod 644 hosts/syslogserver.cfg
cp /vagrant/nagios/services/load.cfg services/
chmod 644 services/load.cfg
service nagios3 reload
```

Now checkout the web app and see if there is a load service on the syslogserver host and then wait to see if it turns green.

Now that you are familiar the most important pieces of Nagios. Go set it up in the Catacombs. :)

