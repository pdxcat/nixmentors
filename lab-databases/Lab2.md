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

Lab 2: Web Services and Databases
=================================

In the last lab we set up a web server to serve content. Web applications often use databases to store and retrieve data. For example, the Wordpress application we set up used MySQL to store user login data and other information. The CAT environment has a number of web applications that use databases, and we also often access databases directly, bypassing a web application. This lab will go over setting up and administering a database, accessing it via the command line, and writing a web application around it.

Definitions
-----------

A **Database** is a collection of data. A **relational database** generally has a number of **tables**, which consists of columns, or fields, and rows, the sets of attributes related to a particular item. **Non-relational databases** are structured very differently and are not in wide use at the CAT. A **schema** is a description of the design of a database. We can also talk about an individual table schema, which is the definition of the table and its fields. We can interact with a database using **SQL**, or **Structured Query Language**, to do things like create new tables, add or modify data in your tables, view data in tables, or delete tables. a **DBMS** or **Database Management System** is the software we use to access structured data. The DBMSs that we will be using today are PostgreSQL and optionally MySQL. Most of what you learn using PostgreSQL can be applied to MySQL and other relational databases such as SQLite, MSSQL, and OracleDB.

There's a lot more to databases than what's above, but that should be enough to get you started. If you think this is interesting, take a look at [Wikipedia](https://en.wikipedia.org/wiki/Database) for more than you could ever realize you never knew about the topic.


Install postgres
----------------

    sudo apt-get update
    sudo apt-get install postgresql

Optional:
    sudo apt-get install vim

Note: alternatively you can type `sudo -i` first, then you don't need to keep typing sudo before each command.

Result:
 - You should have a new command available, psql
 - There is now postgres user
 - The postgresql service should be running

Configuring Postgres
--------------------

### Configuration Files

##### /etc/postgresql/9.1/main/postgresql.conf

This is the main configuration file. Here is where you can set things like:

 - Set log level, log location, logging configuration
 - change port number it runs on
 - change the address it listens for

##### /etc/postgresql/9.1/main/pg_hba.conf

This is the client authentication file. It controls who can connect and from where.

#### Mandatory Setup Instructions

  - Edit `/etc/postgresql/9.1/main/postgresql.conf`. Uncomment the listen_address line and change it from 'localhost' to '*' so we can connect to it from places other than localhost

  - Edit `/etc/postgresql/9.1/main/pg_hba.conf`. Add the following line to allow connections from the host lab computer:

    `host all all 10.0.0.0/8 md5`

  - Restart the postgres server in order to make your changes happen:

    `sudo service postgresql restart`

#### Additional Exercises

 - Following the guides in the comments of postgresql.conf, turn on the logging collector
 - Change the log file name
 - Change the log line prefix
 - Change the logging destination to syslog. Where should you look for logs now?
 - Change the maximum number of connections
 - Reject connections from user blkperl on all databases

### Command line administration

Use `psql` to get into the postgresql command line interface. To configure the database, you need to connect as the user postgres.
    `sudo -u postgres psql`

#### help commands
   - `\?` and `\h`

#### getting information about your system
   - `\dS` shows all the possible databases you have access to, including all of the ones that postgres keeps for itself
   - `\d <database name>` describes what kinds of values are in the database
   - `\dfS` describes all the functions available to you
   - info functions: http://www.postgresql.org/docs/9.2/static/functions-info.html
   - administrative functions: http://www.postgresql.org/docs/9.2/static/functions-admin.html
   - variables: everything set in your configuration file is available as a variable in psql

#### Try It
   - `select * from pg_database;`
   - `select * from pg_stat_activity;`
   - `select * from current_database();`
   - `show all;`


SQL
---


### Basic SQL

   - Relational databases use tables to store their information.
   - Tables have columns and rows.
   - Columns have data types, the same way variables in programming languages have data types. Some common data types are varchar(number), which is like string with a max length, integer, and boolean. Postgres has many possible data types.
   - Usually tables have primary keys. This is a column that will have a unique value for every row.
   - All SQL queries end in a ';'.
   - Strings are enclosed in single quotes.
   - SQL commands are case-insensitive. However, you will often see SQL commands in all-caps which helps delimit them from the things they are operating on. It is good practice to use all-caps for SQL commands when you are writing application code in order to make it more readable. When you are interacting directly with the database via psql, however, it doesn't matter.
   - Some of the most common SQL commands are `SELECT` (display data), `CREATE` (create a new table or database), `INSERT` (add data to table), `DELETE` (remove data from a table), `ALTER` (change settings for tables, databases, users), and `DROP` (delete whole tables, databases, columns, etc.)

#### Exercises

 - create a database

     `create database star_trek;`

 - create a user

     `create user <username>;`

 - give the user a password

      `alter user <username> with encrypted password '<password>';`

 - grant full admin on the database to the user

     `grant all privileges on database star_trek to <username>;`

 - From now on let's use your user to do things instead of the postgres user (the database's root user). We can even connect to your postgresql server remotely. 

      Quit psql:

     `\q`

      Log out of your vagrant vm:

     `exit`

     Connect to postgres on your vagrant vm from your lab computer:

     `$ psql -U <username> -h 127.0.0.1 -p 8543 star_trek`

 - create a table called characters with columns name as the primary key (values in column can't be repeated), rank, position, series, and actor

     `create table characters(name varchar(100) primary key, rank varchar(200), actor varchar(100), position varchar(200));`

 - look at your table's schema (use `\d`)

 - select everything in the table (it will be empty)

     `select * from characters;`

 - import some data into your table. This path is on your host machine - i.e. your lab computer, not your vagrant vm, since we are essentially accessing the database remotely.

      `\copy characters from '/path/to/Lab2/startrek.csv' with csv;`

 - delete one of the characters

      `delete from characters where name='Chakotay';`

 - insert the character back into the table
   - remember to use single quotes!
   - if you're giving every field, in the order you see when you look at the table schema, the syntax is

       `insert into <tablename> values(<list of values>);`

   - if you're only inserting some of the fields, leaving others blank, or inserting in a random order, the syntax is

       `insert into <tablename> (<list,of,fields,separated,by,commas>) values(<list,of,values,corresponding,to,sequence,of,fields>);`

 - select everything from the table again

 - select only names from the table

 - select only ranks or series from the table (what happens if you have multiple characters with the same rank or from the same series?)

 - select all rows WHERE (hint hint) the name of the characters is Spock.

 - alter the table to have an age column:

       `alter table characters add column age integer;`
       `update characters set age = 22 where name = 'Harry Kim';`

#### Advanced SQL

Suppose wanted to insert a new character with the rank of "Lt. Commander". Then we wanted to select all characters with that rank. What happens to the characters that we gave the rank "Lieutenant Commander"?

In addition to restricting columns to be a certain data type, you can put other constraints on your columns. 

What we often do is separate these concerns into separate tables. This prevents us from misspelling things, reduces repetition of identical values, and organizes our data into logical modules. We use foreign keys to tell postgres that the data in a particular column is referring to a row of another table.

Then we can use joins to get data out of both tables with one query.

#### Exercises

   - Option 1: add constraints

        `alter table characters add constraint rank_constraint check (rank = 'Lt. Commander' OR rank = 'Captain' OR rank = 'Commander' [...]);`

   - Option 2: separate values that have order into a different table
     - create the new table called 'ranks' with two columns, id (primary key) and rank
     - insert all your ranks with corresponding ids
     - change your characters table to have the rank column use the integer data type with the ranks table as a foreign key
        - best to drop the old column and recreate with new data type and foreign key constraint

                alter table characters drop column rank;
                alter table characters add column rank_id integer references rank(id);

     - get reasonable data out of both of your tables
	 
            select characters.name, rank.name from characters join rank on characters.rank_id = rank.id;

   - Challenge: Experiment with different constraints. Maybe set a minimum age for characters (who needs Wesley anyways).
   - Challenge: Create another reference table (e.g. series) and alter a column to refer to that table


### Backing Up Databases

Star Trek characters are critical to our infrastructure, so they need to be backed up.
The pg_dump command takes all of the stuff stored in a single database and puts it into a file, so you can copy it to other servers (for example). 
Use psql to import the file into a database.
pg_dump only gets the data from one database. It doesn't get things like roles. pg_dumpall can get all the databases in postgres along with any roles. You can use pg_restore to restore from a pg_dumpall.

Note: This should be done on the vagrant vm as the postgres user.

#### Exercises

 - Back up your database
     `sudo -u postgres pg_dump <databasename> > backup.sql`
 - Drop your database
     `sudo -u postgres echo "drop database <databasename>" | sudo -u postgres psql`
 - Restore it
     `sudo -u postgres psql < backup.sql`
 - yay!

### Writing Applications Against Your Database

The CAT makes a lot of use out of psql, but that's not really how databases are used in the wild. We can write applications against our databases so that we don't have to grab all the data manually.

Write a PHP application to query your database and display the results on a web page. You can either set up Apache and PHP on your VM (port 80 is already forwarded to 8080) or you can put your PHP application in your public\_html in your Tier 1 home directory (/home/username/common/public\_html). 

PHP scripts start wtih <?php and end with ?>.

Database PHP programming: http://www.php.net/manual/en/book.pgsql.php

Connect to the database with the pg\_connect function. Store the return value in a variable so you can use it later.
Query the database with the pg\_query function. The return value of this function is the rows returned from your query.
Fetch your result with the pg\_fetch\_all (or find another function that fetches what you want). You won't be able to get the data from your query without fetching it first.
Display your rows with print\_r.
Close your connection with pg\_close.

Homework: activate your postgres and mysql accounts in crack, create some tables, write a web app that queries them and inserts things into them with prepared statements and error checking, show krinkle (Watch out for [SQL](https://en.wikipedia.org/wiki/SQL_injection) [Injections](http://xkcd.com/327/)!)

Advanced:
 - Repeat everything using mysql
 - Research postgres functions and write one.
 - Add another column to the characters table that has a default value of the current time using the now() function (look up ALTER TABLE).

