Webs
===

⁑ indicates a task to accomplish

⁂ indicates an advanced task that can augment or replace a basic task

code snippets will not be given to advanced students, and maybe not to beginners either

Apache
---
### Setting up the service
⁑ Install apache:

    sudo apt-get install apache2

⁑ Visit http://localhost:8080, and examine ongoing HTTP requests with the browser's inspector.

Either:
- Inspector (Firefox) - Ctrl-Shift-C or right click and select 'Inspect Element'
- Web Inspector (Chrome) - Ctrl-Shift-I or right click and select 'Inspect Element'
    
Select the Network tab in either browser.

Refresh the page
- Ctrl-Shift-R in Firefox, or
- Shift-F5 in Chrome

⁑ Take a look at your web root directory. What's in there? Who owns the files?
    
    cd /var/www/
    
What happens if you rename index.html? What happens if you delete it?

⁑ Put some new content on your server's default page

    vim /var/www/index.html

- Include an image and some text and headers
- Make another page in your web root and link to it from your 
- Extra points for CSS or JavaScript fun.

⁑ Install curl, then use it to get the page source - check out the verbose one too:

    curl http://localhost
    curl http://localhost -v
    
⁂ Communicate with your server using netcat/telnet
___

⁑ How's your server doing?

    sudo service apache2 status

⁑ Start, restart and stop your web server. What do these other actions do?

    sudo service apache2 {start|stop|graceful-stop|restart|reload|force-reload|start-htcacheclean|stop-htcacheclean}

⁑ By default apache will stay on unless you tell it to stop, and it will autostart on boot. Disable apache and ensure it doesn't start back up when you restart your VM:

    sudo update-rc.d apache2 disable

⁑ Now enable it again - I bet you can guess how to do this now

___

### Configuring the server

Look at your apache configuration file

    sudo vim /etc/apache2/apache2.conf

Skim through the file. You can probably guess what a lot of these lines do. Sometimes you'll see a variable like `${APACHE_RUN_USER}`. These are environment variables, which are defined in another file. You can edit them here:

    sudo vim /etc/apache2/envvars

Test out all of these things:

⁑ Change the apache user and group to something else:

Make the new user. This will make a new group for the user as well. What do these options to useradd accomplish? Why would we want to use them?

    sudo useradd -r -s /bin/false rambo

Now change the name of the apache user and group in your envvars file. Restart the service to make the new ownership take effect. 

    sudo service apache2 restart

Figure out how to fix any error messages or warnings it produces.

## Enable port 8081 and 8082

Add two more listen directives in /etc/apache2/ports.conf

    echo "Listen 8081" >> /etc/apache2/ports.conf
    echo "Listen 8082" >> /etc/apache2/ports.conf

Restart apache

    service apache2 restart

Verify the ports are listening with netstat

    # netstat -tlnp | grep apache
    tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN      1448/apache2    
    tcp        0      0 0.0.0.0:8081            0.0.0.0:*               LISTEN      1448/apache2    
    tcp        0      0 0.0.0.0:8082            0.0.0.0:*               LISTEN      1448/apache2    


Install PHP
-----------

    sudo apt-get install php5 libapache2-mod-php5
    sudo a2enmod php5
    sudo service apache2 restart

- Add a php file in the web root (/var/www)

First try the phpinfo() function


```php
<?php
phpinfo();
?>
```

Then check your new webpage to see if the PHP info page renders.

- Next replace `phpinfo()` with `date_sunrise()` and debug why it doesn't work

- Check out /etc/php5/apache2/php.ini
    - Check error reporting, error log
    - Turn on error display
- Now look at the page again and observe the difference in output, also look at log


Setup the default vhost with SSL
--------------------------------

Enable the mod

    a2ensite default-ssl
    a2enmod ssl
    service apache2 reload

Browse to https://localhost:8443/

You should see your customized index page.


Setup a vhost with CAS Auth
---------------------------

Create the casapp

    mkdir /var/www/casapp
    cp /vagrant/casapp/index.php /var/www/casapp/

Copy in the vhost

    cp /vagrant/vhosts/025-casapp /etc/apache2/sites-enabled/025-casapp

Check the vhost syntax

    apachectl -t

Restart Apache

    service apache2 restart

Now browse to http://localhost:8081 and verify the page loads

### Install CAS

Our GlobalSign cert requires the newset version of mod-auth-cas, johnj has packaged it and placed it on our mirror.

    # Download from our mirror
    wget http://mirrors.cat.pdx.edu/cat/pool/main/liba/libapache2-mod-auth-cas/libapache2-mod-auth-cas_1.0.10-cat1_amd64.deb
    
    # Install the package with dpkg
    dpkg -i libapache2-mod-auth-cas_1.0.10-cat1_amd64.deb

    # Enable the mod
    a2enmod auth_cas

    # Check the configs
    apachectl -t

It might fail if libcurl.so is missing

    apt-get install curl

### Modify the vhost

Edit the vhost file you copied over earlier

    vim /etc/apache2/sites-enabled/025-casapp

Add the CAS variables somewhere in the top-level block

    CASLoginURL https://auth.cecs.pdx.edu/cas/login
    CASValidateURL https://auth.cecs.pdx.edu/cas/serviceValidate
    CASCookieDomain pdx.edu

Add a location block

    <Location />
      Authtype CAS
      require valid-user
      CASCookie CECS_AUTH_CAS
      CASSecureCookie CECS_AUTH_CAS_S
      CASGatewayCookie CECS_AUTH_CAS_G
    </Location>

Now browse to http://localhost:8081/ and verify it prompts for your CAT credentials

Note: CAS redirection fails after logging in. Feel free to update the docs if you figure out how to make it work.

Advanced: Your casapp can be accessed via http://localhost:8080/casapp/ effectively bypassing CAS auth. How can we fix this?

Wordpress
---------

Install wordpress from https://wordpress.org/download/

The official docs are here:

https://codex.wordpress.org/Installing_WordPress

When it tells you to edit wp-config.php, set the database name and username both to "wordpress" and the password to "hunter2". Before you open it in your browser to finish the install, pause to set up the database.

## Install Mysql

    # Be sure to remember the root password you set
    apt-get install mysql-server
    apt-get install php5-mysql
    service apache2 restart

### Connect to mysql

    mysql -p

### Create a table and user 

```mysql
CREATE DATABASE wordpress;
GRANT ALL PRIVILEGES ON wordpress.* TO "wordpress"@"localhost" IDENTIFIED BY "hunter2";
FLUSH PRIVILEGES;
exit
```

### Test that it works

    mysql -u wordpress -p

## Continue with your installation

http://localhost:8080/wordpress/

## Configure wordpress with suPHP

By default, PHP scripts are run with the permissions of the apache user (or root). SuPHP is an Apache module which can instead run them as the user who owns them. This is great for environments like MCECS, where there are a lot of different users and we can't assume they all know what they're doing. To get it running, you'll need to install the package, enable the module, and configure a vhost. Then you can make a wordpress user and chown the wordpress files to it.

Let's begin by removing the default vhosts

### Disable the default vhosts

    a2dissite default
    a2dissite default-ssl
    # You may wish to remove the CAS vhost as well
    rm /etc/apache2/sites-enabled/025-casapp

Here is the vhost from braindump.cat.pdx.edu, adapt to it work for your wordpress. Copy it into a file named  `/etc/apache2/sites-enabled/025-wordpress` and modify it so that it works for your new wordpress.

```
<VirtualHost 131.252.208.58:8000 [2610:10:20:208::58]:8000>
    ServerName braindump.cat.pdx.edu
    DocumentRoot /www/braindump

    # no public_html
    UserDir disabled

    # Turn on suphp for this vhost
    AddHandler x-httpd-php .php .php3 .php4 .php5 .phtml
    suPHP_AddHandler x-httpd-php
    suPHP_Engine on
    suPHP_ConfigPath /etc/php5/apache2

    <Directory "/www/braindump">
        Options Includes ExecCGI Indexes All
        AllowOverride All
        Order allow,deny
        Allow from all
    </Directory>
</VirtualHost>
```

There are several things that need to be changed for this vhost file to work for your wordpress. Make sure to check the syntax with `apachectl -t` after editing the file. We recommend getting the vhost working without the suPHP options before trying to get suPHP working.
