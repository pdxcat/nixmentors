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

Nix Mentor Sessions
===================

[![Build Status](https://travis-ci.org/pdxcat/nixmentors.svg?branch=master)](https://travis-ci.org/pdxcat/nixmentors)

This repository is home to the materials used in the Nix Mentor Sessions run by [theCAT](http://cat.pdx.edu) for the 2014 [Braindump](http://braindump.cat.pdx.edu) year.


Sessions
--------

* Lab 1: [Intro to Vagrant and Intro to Web](lab-intro-web-vagrant/README.md)
* Lab 2: [Databases and Advanced web](lab-databases/README.md)
* Lab 2.5: [Intro to Git](lab-git/README.md)
* Lab 3: [NFS](lab-nfs/README.md)
* Lab 4: [Monitoring](lab-monitoring/README.md)
* Lab 5: [Storage](lab-storage/README.md)

Setting up Vagrant
==================

Initial setup
-------------

```bash
mkdir -p /disk/trump/minerals/$USER
vboxmanage setproperty machinefolder /disk/trump/minerals/$USER
```

Clone the Repo
--------------

```bash
git clone https://github.com/pdxcat/nixmentors.git
cd nixmentors
cp .sample-nixmentors.yml .nixmentors.yml
```

Lab 1
-----

```bash
cd lab-intro-web-vagrant
vagrant up
vagrant ssh
sudo -i
```

Now proceed to the [First Lab](lab-intro-web-vagrant/README.md) instructions.

Note that this will install the base trusty64 image into the user's homedir. It will consume 300MB of profile space.

Additional Instructions for Alumni accounts
-------------------------------------------

If you have been alumnified, this will fail because there's not enough space. You can store the image somewhere else instead. Add the following environment variable to your .whateverrc:

    export VAGRANT_HOME=/disk/trump/minerals/$USER

Note that if you try to do `vagrant up` on multiple machines, you will probably get an error about an inaccessible vm. If that happens, do like so:

    VBoxManage list vms

You'll see a line identifying a vm that's inaccessible, and a hash to identify it. Take that entire hash (minus the curly braces), and run this:

    VBoxManage unregistervm <hash>


At the end of the lab
---------------------

```bash
# exit vagrant ssh
vagrant halt
```
