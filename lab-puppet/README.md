Lab 6: Puppet
=============

Pre-Lab
-------

Puppet is a way to automate configuration of hosts. Before learning Puppet, it is important to know how to configure a host by hand (learn to walk before you can run, so to speak). Make sure you know how to do the following tasks. You can use the VMs provided in this lab, your laptop, or machines in the catacombs. Explicit instructions are not provided, so please take advantage of Google and IRC if you do not know how to do these things.

- Create a user.
- Add the user's public SSH key.
- Create a group. Make the user a part of that group.
- Create a file in /tmp that is owned by the user you created, is readable and executable by the group you created, and is not readable to anyone else.
- Schedule a cron job to add the line "I am a cron job" to the end of a file every hour.

You can refer to Lab 4 to do these:

- Install the rsyslog package
- Change the rsyslog configuration file
- Restart the rsyslog service

Skip this if you have a working runaway script. You can use the linux lab machines for this (or ask about netgrouplist on IRC):

- Write an SSH for-loop to create a file containing "$USER was here" in /tmp

What is Puppet?
---------------

If you did the pre-lab, you can see that setting up some basic configuration takes a bit of time and effort. On one or two hosts, that's not a big deal. It becomes a problem when you have hundreds or thousands of hosts to manage. The name of the general solution to this problem is **configuration management**. There are several different software products that provide configuration management. Puppet is the configuration management tool used at the CAT.

### Why not use an SSH for-loop to do this?

An SSH for-loop is a sysadmin's Swiss Army knife for Getting Stuff Done. We use it a lot at the CAT. The idea is to write a bash script to SSH in to a number of hosts and do some configuration on each one. This is a basic form of automation, and for many cases, this works great. So what does Puppet get us that SSH for-loops don't?

The keyword that configuration management hipsters love to use is **[idempotency](http://en.wikipedia.org/wiki/Idempotence)**. It means that if an action needs to be performed to maintain a certain state, the action will be done. If the system is already in the desired state, Puppet doesn't try to reapply configuration, which might result in errors. This means that you don't get errors from trying to ensure something is the way you want it -- e.g. you don't have to run `service apache2 start` twice, which would result in an error the second time.

### Puppet Code

The Puppet software is written in Ruby. The only reason this is important is because you will need to have an up-to-date version of Ruby installed. Otherwise, you would do best to forget what language it is written in. The Puppet code you will be writing is a **Domain-Specific Language (DSL)**. It is NOT Ruby, nor does it really have many similarities to Ruby. The Puppet language is a **declarative** language. This means that code is written to describe a state without describing how to get there. This contrasts with an **imperative** language, in which the programmer gives instructions or a recipe to tell the computer how to reach some state. The difference will hopefully become more clear after this lab.

### Learning Puppet

Puppet is pretty complicated. Puppet Labs charges a lot of money for three-day long training courses, and that just covers the basics. This lab is meant to get you used to the idea of the Puppet setup, resources, and modules. It's not going to cover everything. Puppet Labs has a great [free tutorial](http://docs.puppetlabs.com/learning) that you can use to cover Puppet more in-depth. The tutorial uses Puppet Enterprise, the closed-source version of Puppet, so some things will be slightly different than what you learn here, but the overall concepts are the same as open-source Puppet and the tutorial is much more complete.

Setting Up a Master and Agent
-----------------------------

Bring up your two machines in the Lab6-Puppet directory.

```
$ vagrant up
```

SSH in to both of them. It will probably be helpful to have two terminal windows open.

```
$ vagrant ssh puppetmaster
$ vagrant ssh client
```

### Configure DNS

Add the following to your /etc/hosts file on the Puppet master:

```
192.168.1.11 client.local client
```

Add the following to your /etc/hosts file on the client:

```
192.168.1.10 puppet.local puppet
```

### Install Puppet

Add the PuppetLabs apt repository to the master and client:

```
$ wget https://apt.puppetlabs.com/puppetlabs-release-trusty.deb
$ sudo dpkg -i puppetlabs-release-trusty.deb
$ sudo apt-get update
```

Install the Puppet master

On the puppetmaster machine, install the Puppet master and a webserver:
```
$ sudo apt-get install puppetmaster-passenger
```

The Puppet client-server model is a lot like the Apache client-server model. The Puppet master runs a web server. Puppet clients run a daemon, called an agent, that periodically makes web requests to the Puppet master. Installing puppetmaster-passenger automatically installs Apache and configures a vhost for us.

On the client, install the Puppet agent:

```
$ sudo apt-get install puppet
```

### Sign Certs

The client needs to request a certificate from the master. By default, it will automatically request a certificate the first time the agent runs. Trigger it by running

```
$ sudo puppet agent --test
```

View outstanding certificate requests on the master with

```
$ sudo puppet cert list
```

It is important to verify this certificate request! Anyone could request a certificate. It's important that certs are only signed for hosts you trust. Make sure the fingerprint next to the host in the list on the master matches the fingerprint that was output on the client.

Sign the certificate with

```
$ sudo puppet cert sign client
```

Verify it worked by running the agent again:

```
$ sudo puppet agent --enable
$ sudo puppet agent --test
```

We have successfully configured a Puppet master and client. We now need to write Puppet code to configure our client.



Resources
---------

Puppet organizes configuration into units called resources. There are many different types of resources. Some examples are:

- a user account
- a group
- a configuration file
- a cron job
- a software package
- a service

The full list of resources Puppet can manage and how it can configure them can be found [in the Puppet documentation](http://docs.puppetlabs.com/references/latest/type.html). You don't need to worry about most of them.

Let's try one out. Let's create a user on our system.

As root, create and open the file `/etc/puppet/manifests/site.pp` **on the Puppet master** in your editor. Add the following to it:

```
user { 'krinkle':
  ensure     => present,
  comment    => 'krinkle is awesome',
  managehome => true,
  home       => '/home/krinkle',
  shell      => '/bin/bash',
}
```

If you look at the entry for the vagrant user in /etc/passwd, some of these attributes should look familiar. Creating a user resource with Puppet is the same as using the adduser command on an Ubuntu system.

Now apply the configuration to the client. Run:

```
$ sudo puppet agent --test --noop
```

At this point let me point out a few things. The typical mode for a Puppet agent is to run as a daemon and check in with the Puppet master every thirty or so minutes to see if new configuration is available. Since we don't want to wait that long for the agent to check in on its own, we run `puppet agent --test` to trigger it right now, and show us the output. We also use the `--noop` flag to tell Puppet to not actually make any real changes *yet*. We want to see the output and make sure it is what we expect before we make any real changes. If we run Puppet without --noop and we realize we've made a mistake, there is not necessarily a simple way to roll back.

If there is a problem with your configuration (such as a syntax error), Puppet will tell you in red.

Once you are sure that you have not made any syntax mistakes and that you are really, really sure you want to create this user, run the agent for real:

```
$ sudo puppet agent --test
```

User krinkle should now exist on the client. You can check by looking in the file `/etc/passwd` or using the command `getent passwd krinkle`. If we had more clients, the Puppet agent on all of them would eventually get this resource from the Puppet master and all the clients would have user krinkle on them.

Let's try another resource. Add the following to the site.pp file on the Puppet master:

```
file { '/home/krinkle/.plan':
  ensure    => file,
  content   => 'krinkle\'s plan',
  owner     => 'krinkle',
  mode      => '0644',
}
```

This will create a file called /home/krinkle/.plan. It will ensure that the file is a regular file, as opposed to a directory or symlink. It will give it some content. It will give it an owner, and set permissions on it. Run the agent on your client again to make the file come into existence.

### Resource Dependencies

Notice that this file resource depended on the user resource already existing. What happens if the user krinkle doesn't exist? Try removing the krinkle user by changing the ensure line to `ensure => absent` in the user resource and running Puppet again.

We can't and shouldn't depend on Puppet to figure out this dependency. We can't even depend on Puppet to read site.pp from top to bottom, so it doesn't matter that the user resource was declared before the file resource. (In practice, Puppet can kind of figure this out, but when you have a complex Puppet ecosystem you should not depend on it.) We need to tell Puppet explicitly what depends on what. One way to do this is with the require attribute. Change the ensure attribute back to present for the user resource. Then make your file resource look like this:

```
file { '/home/krinkle/.plan':
  ensure    => file,
  content   => 'krinkle\'s plan',
  owner     => 'krinkle',
  mode      => '0644',
  require   => User['krinkle'],
}
```

Notice the capitalized User. Puppet uses this notation when it wants to refer to a resource that is declared somewhere else. In this case it is looking up a user called 'krinkle' in its collection of user resources. It tells Puppet to make sure to create the file resource after the 'krinkle' user is created.

Delete the file and the user from your client. Make sure that re-running Puppet properly creates the user and the file.

You can do this going the other way. Remove the require line from the file resource, and make your user resource look like this:

```
user { 'krinkle':
  ensure     => present,
  comment    => 'krinkle is awesome',
  managehome => true,
  home       => '/home/krinkle',
  shell      => '/bin/bash',
  before     => File['/home/krinkle/.plan'],
}
```

These two ordering attributes accomplish the same thing. Which one you use depends on what makes sense in a particular context.

There is an alternate syntax for resource ordering, [described in the documentation](http://docs.puppetlabs.com/learning/ordering.html).

#### Exercise

(Feel free to move past this and come back to it later since this will be a long lab.)

Task: use the [ssh_authorized_key](http://docs.puppetlabs.com/references/latest/type.html#sshauthorizedkey) resource to manage krinkle's public key. You can use your own public key or generate one specially for krinkle. Then put it in Puppet.

## Package, File, Service

You might have noticed from previous labs that configuring a server to do a particular task often involves three things:

- installing a package from a repository
- changing a configuration file to meet our particular needs
- starting or restarting the service

While Puppet provides us with many resources, the package, file, and service resources are absolutely the most important ones. More than anything else, configuring a server involves these three parts. 

Since we have already played with [rsyslog in a different lab](../Lab4-Monitoring/Lab4.md), let's try configuring it with Puppet. Tutorials will commonly have you configure either SSH or NTP as an introduction to Package-File-Service. If we accidently break SSH on our vagrant vm, it can be painful to fix. And NTP is boring. So we're doing rsyslog.

Add a package resource for rsyslog to site.pp:

```
package { 'rsyslog':
  ensure => installed,
}
```

This will install the service for us, but it won't configure the file for us. We could write the configuration file from scratch, or we could take the one that rsyslog installs by default, copy it into a place that Puppet can access, and make the changes we need to it. Let's do that: on the client, copy /etc/rsyslog.conf into /home/vagrant/puppet/ (you will have to create the directory puppet in /home/vagrant). Make the changes indicated in Lab 4. You can either configure this to be a syslog client or server, your choice. Make one additional change: add a comment to the top of the file indicating that this file is managed by Puppet, so that everyone knows that this file is managed by Puppet and that Puppet will undo any manual changes made to it. Then add a file resource to site.pp on the Puppet master:

```
file { '/etc/rsyslog.conf':
  ensure => file,
  source => '/home/vagrant/puppet/rsyslog.conf',
}
```

This will tell Puppet to look for a file at /home/vagrant/puppet/rsyslog.conf on the client, and to place it at /etc/rsyslog.conf.

Now we need to manage the rsyslog service. Add a service resource to site.pp on the Puppet master:

```
service { 'rsyslog':
  ensure => running,
  enable => true,
}
```

This tells Puppet to make sure that the rsyslog daemon is running and that it will be started on boot.

We have a package, file, and service, but we're not finished yet. Like before, these three resources depend on one another in specific ways. But now, not only does one have to exist  before another, but we actually want the state of one to change when the state of another changes. If we make any changes to the configuration file, the rsyslog service needs to be restarted, otherwise it wouldn't pick up the changes. We have two new attributes to manage this kind of dependency: subscribe and notify.

Let's try subscribe first. Change your service resource to look like this:

```
service { 'rsyslog':
  ensure    => running,
  enable    => true,
  subscribe => File['/etc/rsyslog.conf'],
}
```

Subscribe is analogous to require, except that not only does it require the file to exist, it will also restart the service if the file ever changes.

Before we try notify, let's get all of these resources to work. There is still a dependency between the package and the configuration file: we want to install the package before trying to change the configuration file (otherwise installing the package would change the file back to default). Add a before attribute to the package resource (or, alternatively, add a require attribute to the file resource).

Now you can run the agent on the client and see if it works.

Now try using notify instead of subscribe. Notify is analogous to before. If the file changes, it will tell the service to restart. Remove the subscribe attribute from the service resource and make your file resource look like this:

```
file { '/etc/rsyslog.conf':
  ensure => file,
  source => '/home/vagrant/puppet/rsyslog.conf',
  notify => Service['rsyslog'],
}
```

Run Puppet on the client again to make sure it still works. Try changing or deleting the file in /etc/rsyslog.conf. When you run Puppet again, what happens?

If you feel comfortable with this Package-File-Service concept, you have mastered the core of what it means to do configuration management. The rest is fluff and syntax.

Nodes
-----

So far we have been putting all of our code in site.pp. This works if you want the same configuration to apply to every hosts in your network, but often that won't be the case. site.pp is usually the place where we list nodes, each of which gets configuration specific to it. Alternatively, you can specify your nodes in an [external node classifier](http://docs.puppetlabs.com/guides/external_nodes.html), but this is much more advanced and we won't go into it here.

In site.pp on the Puppet master, enclose your resources in a node statement like this:

```
node 'client' {
   # Resources
}
```

If you had additional nodes, you would give each a similar node definition. You could use a regular expression or a comma-separated list of nodes to group nodes together, if you like. Feel free to add more hosts to the Vagrantfile if you want to play with more than one node.

Classes
-------

You can get a lot done just by knowing about resources and how to string them together. Once you get more nodes and more services, declaring all your resources can get messy and could mean hundreds of lines of code for each node. We split off resources into classes to clean things up.

**Define** a class in site.pp on the Puppet master (outside of the node definition):

```
class syslog {
}
```

Copy all the resources related to syslog from the node definition into the class, inbetween the curly braces. Remove them from the node definition.

Classes in Puppet are kind of like classes in other language. Right now we just have a class definition. It won't do anything by itself, in the same way a class in other languages won't do anything until you make an instance of it. You can uninstall rsyslog or modify your rsyslog.conf and rerun Puppet to see it not doing anything.

To make it do something, you have to **declare** it. Add this line to your node definition, where you used to have syslog resources:

```
include syslog
```

Rerun Puppet on your client and make sure your syslog server is properly configured.

Including this class on our node makes the node definition easier to read: we can tell that this node is a syslog server without having to know how syslog is configured.

Modules
-------

Classes shouldn't actually be defined in site.pp. Having it there still causes site.pp to look long and messy. Instead, we put classes in modules.

A **module** is like a Puppet package, kind of like gems are Ruby packages or RPMs are RedHat packages. It is a bundle of code, data, and metadata that gets run through the Puppet application and output as server configuration. Modules are used to organize your classes and files into logical units. A module has a very specific directory structure. The two parts that matter right now are the manifests directory and the files directory.

- **manifests/** - Where your Puppet code goes. A manifest is like a Puppet program.
- **files/** - Where configuration files, like rsyslog.conf, go. Putting your files in a Puppet module like this makes it so that the file doesn't have to already exist on the client, which is what we've been doing until now.

Let's make a module! In /etc/puppet/modules on the Puppet master, make a directory called syslog. In that directory, make directories called manifests and files.

In the manifests directory, make a file called init.pp. This file is kind of like the main() function in other languages. If there are other manifests in your manifests/ directory, Puppet knows to look at init.pp first. Copy the syslog class from your site.pp into the init.pp manifest. Remove it from site.pp.

Mess up your client configuration by destroying /etc/rsyslog.conf or uninstalling the package. Run Puppet again to make sure your syslog server is restored.

To finish the module, we need get the syslog configuration file out of vagrant's homedir on the client and into Puppet. Copy the config file from the client to the Puppet master:

```
$ scp client:/home/vagrant/puppet/rsyslog.conf /etc/puppet/modules/syslog/files/
```
Remove the file from /home/vagrant/puppet on the client. We don't need it there, nor would we want to depend on it being there.

Now, however, if we run Puppet again, we will get an error. Take a look at the file resource in the syslog manifest: it still refers to the path /home/vagrant/puppet/rsyslog.conf on the client, which is now incorrect. In order to get it to look on the Puppet master for the file, change the source attribute of the file resource to look like this:

```
file { '/etc/rsyslog.conf':
  ensure => file,
  source => 'puppet:///modules/syslog/rsyslog.conf',
}
```
The URI scheme "puppet:///" (yes, there are three '/'s) tells Puppet to look for the file on the Puppet master. The first part of the path needs to be modules/, then the name of the module, and then the path of the file relative to the files/ directory. Notice that the directory files/ is not included in this URI. Puppet knows that files come from files/, so you don't need to specify it.

Try running Puppet on the client again to make sure there are no errors.

### Exercises

- Write a module called plan to add the user krinkle and krinkle's .plan file. Remove those resources from site.pp and include the new module on the client node.

Optional (or come back to these later):
- Write a module to configure NTP.
- Write a module to configure Postgres.

Variables
---------

Puppet can use variables. Variables in Puppet are prepended with a $.

In your plan module, define a variable:

```
$username = 'blkperl'
```

Use the notify resource to test out your variable:

```
notify { "The user's name is $username": }
```

Run Puppet on your client. The notify resource is like a print statement in other languages. It can print out messages, but makes no configurations changes. Notice the use of double-quotes for this string. We use double-quotes here because the string needs to be interpolated, that is, the variable needs to be evaluated rather than printed as literally "$username". Try it with single-quotes to see the difference.

A variable on its own is not very helpful. We can use the variable in a resource to make the resource more dynamic. Change the name of your user resource to $username. Change all other references to krinkle to $username. Don't forget to change your quotes:

```
user { $username:
  ensure     => present,
  comment    => "$username is awesome",
  managehome => true,
  home       => "/home/$username",
  shell      => '/bin/bash',
  before     => File["/home/$username/.plan"],
}
```

You'll have to change your file resource as well. Run Puppet on your client to create the new blkperl user.

### Parameters in Classes

Classes can accept variables as parameters, so a module can be made to behave in different ways depending on its parameters.

Change your plan class to look like this:

```
class plan (
  $username,
) {
  # Resources ...
}
```

Remove the variable definition line from the class. The variable is now being passed into the class.

Open site.pp. Change the line that says `include plan` to look like this:

```
class { 'plan':
  username => 'nightfly',
}
```

The syntax for declaring a class now looks as if we're declaring a resource. We specify the value for $username by making username an attribute of this "resource".

Run Puppet on your client to create the new user.

### Facter

Puppet uses several built-in variables to provide information about a node. These are useful for finding out things like the node's operating system, architecture, IP address, or other properties that a node has that we don't want to define ourselves. Puppet uses a separate utility called facter to get this information. You can play with facter yourself. If you run `facter` on a node, it should print out everything it knows about the node. You can run facter with a single argument from the list of facts to print out that particular fact, e.g. `facter osfamily`.

To use these values within Puppet, you just use a variable like `$::osfamily`. The double-colon at the beginning means this is a "top-scope" variable, which just means that the variable is available anywhere in your Puppet code. We won't talk much more about scope here.

Often you will want your module to change behavior depending on the value of a variable or a fact. For instance, if you are writing an SSH module, the SSH service is called "ssh" on Debian systems and "sshd" on Redhat systems. You can use the $::osfamily fact and conditional statements to change the name of the service depending on the type of operating system the agent is running on. A conditional is something like an if statement or case statement and should be familiar to you from other languages. There are too many types of conditionals in Puppet to discuss them here, but they are fairly self-explanatory and discussed in [the learning documentation](http://docs.puppetlabs.com/learning/variables.html#conditional-statements).

#### Exercises

- Use a notify resource in one of your modules to examine the values of `$::osfamily`, `$::operatingsystem`, `$::hostname`, and `$::fqdn`.
- In your plan module, make the content of the plan file contain a fact.

Templates
---------

Now that you know about variables, we can talk about templates. Templates are the same as files, except they are able to take variables from your manifests in order to generate different files based on different criteria. Templates live in the templates/ directory instead of the files/ directory. A file resource refers to a template with something like `content => template('syslog/rsyslog.conf.erb')` rather than with the source attribute. Puppet templates are written in the ERB templating language, which is plain text with embedded Ruby (I lied slightly about Ruby's importance to writing Puppet code).

In your plan module, make a directory called templates/. In it, make a file called plan.erb with the contents:

```
This is <%= @username %>'s plan!
```

You can use any variables from your manifest or facter in your template, but here they are preceded by "@" instead of "$". Feel free to add more content and variables to your template.

Change the file resource in your plan module to refer to the template instead of a single string:

```
file { "/home/$username/.plan":
  # ...
  content   => template('plan/plan.erb'),
  # ...
}
```

Using the template function is different from specifying a source attribute. It still knows to look in templates/ for templates, but you do not have to start the file path with puppet:/// nor do you have to specify the modules/ part of the path.

Run Puppet again to see your template get applied, with the variables substituted for the values you defined for them!

Defined Types
-------------

The last major topic to be aware of is defined types. To explain defined types, we first need to give a better explanation of classes.

Classes are declared on a node exactly once. You can't, for instance, declare the syslog class twice on a node because you wouldn't (typically) have two syslog servers running at the same time on a node. Sometimes, however, we want to have multiples of something. For instance, if we defined an Apache vhost class, we would potentially want to use it more than once on a single node, since it makes sense to have more than one vhost on a node. Classes won't let us do this. Instead, we can create new, custom resources, that we can use more than once. We call these **defined types** or **defined resource types**. We define them a lot like a class, but you can think of it as more like a resource akin to a file resource, package resource, etc. They are *custom* resources.

Since we might want to have more than one user with a plan on a given node, let's change our plan class into a defined type. All you need to do is change the keyword `class` to `define`. Then in your node definition, you need to declare your new type like this:

```
plan { 'askore':
  username => 'askore',
}
```

The text before the ":" can be referred to in your manifest by the variable `$title`. You could replace instances of $username with $title, or you could set the default value of $username to $title, since it makes sense for them to be the same here (this is back in the plan manifest):

```
define plan(
  $username = $title
) {
  #...
```

Then you can just say `plan { 'askore': }` in your node definition. You can now define multiple plans. Add a nightfly plan and a blkperl plan.

Post-Lab
--------

We touched on several topics but did not go into a lot of depth on any of them. There are many details and gotchas that we didn't touch on. Check out the tutorial linked above for a more complete learning experience.

Ask a claw to add your public SSH key to nightshade so that you can clone the CAT's Puppet repository and check out our code. When you see USN/RHN tickets, ask us how to complete them with Puppet.

There are other products and services that go along with Puppet. Research and set these things up in the combs:

Most important:
- Use a forge module to configure a service (http://forge.puppetlabs.com)
- Use hiera to store your data
- Learn about writing custom facts, functions, types, and providers
- Learn about testing your modules with rspec-puppet and beaker

Other cool services:
- r10k
- external node classifiers
- mcollective
- puppetdb
- puppetboard
- foreman
