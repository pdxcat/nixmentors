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

```bash
vagrant up
vagrant ssh
```

Section 1: Syslog
-----------------

Syslog is used by services to log errors and other information.

Install rsyslog, a leading syslog implementation, on all of the systems:
```bash
sudo yum install rsyslog
```

Modify the rsyslog configuration file: /etc/rsyslog.conf

On the server, uncomment these lines:
```bash
 #$ModLoad imudp
 #$UDPServerRun 514
```

On the clients, add these lines:
```bash
* @syslogserver
```

Add the server to the /etc/hosts file on the clients:
```bash
192.168.1.10 syslogserver
```

Allow traffic in the server's firewalling rules, /etc/sysconfig/iptables

Add the following line before "-A INPUT -j REJECT":
```bash
-A INPUT -p udp --dport 514 -j ACCEPT
```

Restore from the configuration file:
```bash
sudo iptables-restore /etc/sysconfig/iptables
```

Restart rsyslog on each of the systems:
```bash
sudo service rsyslog restart
```

Watch the logs on the server:
```bash
tail -F /var/log/messages
```

While sending test messages to syslog from the clients:
```bash
logger test
```

You should see your message added to the log file on the syslog server.

###Section 1.1: Log Rotation

Now log rotation will be configured to organize the log files on
syslogserver. Take a look at the manpage of logrotate and the default
file "/etc/logrotate.d/rsyslog".

* Configure logrotate to move old files to a different directory
* Change the way logrotate renames files to include the date
* Have logrotate log to syslog when it has finished

You can use the following command to force log rotation:
```bash
logrotate -f /etc/logrotate.d/rsyslog
```

Section 2: Nagios
-------------------

Nagios is a popular open source monitoring tool designed to let system administrators know about problems in their infrastructure. In this lab Nagios will be monitoring the infrastructure on your vagrant vm.

To get started point your web browser at `http://<your_vm_ip_address>/nagios3`. Then click on "Hosts (Unhandled)". You will see a lot of broken services. Your job is to fix them all by logging into the machine, finding the problem, performing a fix and then checking with Nagios to see if the service turns green.

