mdadm-advanced
==============


Welcome
-------

Did I ever tell you about the time I broke my leg looking for a good stick
to use for training my frog how to fetch?

Welcome to *mdadm*

What are you talking about?
---------------------------

That is a great question that has mystified the greatest minds of my brother's
girlfriends second grade students. Luckily our highly developed thinkerators can
easily compress the comprehensive computer disk creating comprehension, into an
impressive image, who's implications imply incredible inclinations, for instance.
Let me go off on a slight tangent for at most three paragraphs.

Let's just say for sake of salaciousness, We have a impressive array of redundant
disks at our disposal. We could... just go ahead and throw these suckers into a
cabinet and forget about them for an eternity. We could glue them together and
turn them into a mosaic piece that expresses our inner battles with adulthood.
Or! We can do something remotely useful like sticking them into a big ol' server
that will hold say 47 of these puppies at once. We would then proceed to call this
contraption 'Just a Bunch Of Disks' or... a JBOD.

Once said JBOD is constructed, we would like to have an easy way to interface with
our new storage solution. Sure we could save random things to random places across
the array, but that would quickly turn into a large disorganized system of storage.
If only we had some way to combine the disks... If only we had a way to make two disks
hold the *exact* same information, so that if we lost on of the disks, we could still
have all the information still sitting on the other. Or, say we wanted speed.. We
could combine two disks in such a way that storing a file to them meant splitting that
file into pieces and storing a section on a single disk. That way we could access the
file as quickly as we can read the biggest piece. Isn't that cool? No. It's freaking
magnexcellent. Look it up. It's true.

What I'm trying to get at is the idea that once you have a JBOD, its not really in
your best interest to use each piece as a separate mechanism. What a true uber leet
sysAdmin will do is create a strange overly complicated combination of this disks into
a single easy to use/access/store filesystem that does a bunch of neat stuff including
be very fast, efficient and redundant so we can look really good in the eyes of our bosses.

ENTER MDADM
-----------

Seriously though I'll tell you what this is now. mdadm is our swiss army tool for
constructing RAID. Now.. I'm assuming you know what a RAID is especially levels
0(stripe), 1(mirror), 5(stripe and parity), and 10(mirrored stripe) are. So what
I'm going to focus on in this tutorial is how to go about constructing software
RAID with mdadm. First we will need disks, so I'll show you how to spin up a few
fake disks, using files that will emulate disks. Then I'll show you how to make some
basic raid setups, probably one of each of the above. Then, I'll take you through
some of mdadm's cool features, including hotspares, logging and Incremental assembly.



Let's make Somes disks
----------------------

All of everything that we are about to do is going to require us to be root. so...
Root up:

`sudo su`

Do you feel that? The power? Well just remember what uncle Ben told us:

`With straight floweres runs blank respun stability`

sorry... I had a mouth full of fish sticks. Just remember you are root. You need to
double check everything you do everywhere all the time.

Let's now make a couple, say 5, blank files that we can use later on as disks. We do
this with a tool called `dd`. Now, many people would call me crazy, telling you to
go straight from root to `dd` in the amount of time im doing now. Mainly because `dd`
has this really cool ability to COMPLETELY OBLITERATE EVERYTHING IN ITS PATH. So if you
pay attention to only one thing this entire tutorial it is this. DOUBLE CHECK `dd` every
time you use it.

Okay, those disks. You can run:

`dd if=/dev/zero of=./hdd1 bs=1M count=1000`

to make a single file, filled with zeros that will be 1G in size. But we have to make a
few so lets put this in a for loop:

```
for i in `seq 5`; do dd if=/dev/zero of=./hdd$i bs=1M count=1000; done
```

OH SHIT WAIT NO DON'T DO THAT!!!!!!!!!!!!!!!!!!!!!!!!!

Just kidding. Just trying to keep you on your toes. You double checked that command
right? I hope so.

You should now have a directory full of blank files that look like the following:

```
[root@fatdadd disks]# ls
hdd1  hdd2  hdd3  hdd4  hdd5
```

This is a good thing. Now that we have our files we can turn them into pseudo-disks
by using this other command called `losetup`. What this command does is 'Setup and
control loopback devices'. Basically it will take a file and make it pretend to be a
'block device' which is a fancy way to describe harddrives. There is more to that
statement that I'm going to leave to you to investigate. lets turn all of those files
into loop devices. Of course we are going to do this in a for loop.

```
for i in `seq 5`; do losetup /dev/loop$i ./disk$i; done
```

Now we can see the loop devices listed in `/dev` like so:

```
[root@fatdadd disks]# losetup
NAME       SIZELIMIT OFFSET AUTOCLEAR RO BACK-FILE
/dev/loop1         0      0         0  0 /root/disks/hdd1
/dev/loop2         0      0         0  0 /root/disks/hdd2
/dev/loop3         0      0         0  0 /root/disks/hdd3
/dev/loop4         0      0         0  0 /root/disks/hdd4
/dev/loop5         0      0         0  0 /root/disks/hdd5
```

Okay, now that we have some disks, we can play
----------------------------------------------

Houston, we have disks...

```sh
[root@fatdadd disks]# ls
hdd1  hdd2  hdd3  hdd4  hdd5
```

`provide examples of losetup -> mdadm to stripe and mirror`


```sh
[root@fatdadd disks]# mkfs.ext4 /dev/md0
    mke2fs 1.42.8 (20-Jun-2013)
        Discarding device blocks: done
        Filesystem label=
        OS type: Linux
        Block size=4096 (log=2)
    Fragment size=4096 (log=2)
        Stride=128 blocks, Stripe width=256 blocks
        128000 inodes, 511232 blocks
        25561 blocks (5.00%) reserved for the super user
        First data block=0
        Maximum filesystem blocks=524288000
        16 block groups
        32768 blocks per group, 32768 fragments per group
        8000 inodes per group
        Superblock backups stored on blocks:
        32768, 98304, 163840, 229376, 294912

        Allocating group tables: done
        Writing inode tables: done
        Creating journal (8192 blocks): done
                                        Writing superblocks and filesystem accounting information: done

```

Now, we can mount it

```sh
[root@fatdadd disks]# mount /dev/md0 /mnt
```

Yeeeeee! So let's reflect.
We have a Two sets of striped file mirrored together. Right?
So Cool. Here is our mirror.

```sh
[root@fatdadd disks]# mdadm --detail /dev/md0
/dev/md0:
Version : 1.2
Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
    Used Dev Size : 2044928 (1997.34 MiB 2094.01 MB)
        Raid Devices : 2
        Total Devices : 2
        Persistence : Superblock is persistent

        Update Time : Sun Feb  2 20:44:53 2014
        State : clean
        Active Devices : 2
        Working Devices : 2
        Failed Devices : 0
        Spare Devices : 0

Name : fatdadd:0  (local to host fatdadd)
    UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
    Events : 17

    Number   Major   Minor   RaidDevice State
    0       9        1        0      active sync   /dev/md1
    1       9        2        1      active sync   /dev/md2
```

And here is one of our stripes

```sh

[root@fatdadd disks]# mdadm --examine /dev/md1
    /dev/md1:
        Magic : a92b4efc
        Version : 1.2
        Feature Map : 0x0
        Array UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
        Name : fatdadd:0  (local to host fatdadd)
        Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Raid Devices : 2

        Avail Dev Size : 4089856 (1997.34 MiB 2094.01 MB)
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
        Data Offset : 2048 sectors
        Super Offset : 8 sectors
        Unused Space : before=1960 sectors, after=0 sectors
        State : clean
        Device UUID : b92a859e:fbf80bb3:8fe608a0:27533392

        Update Time : Sun Feb  2 20:44:53 2014
        Bad Block Log : 512 entries available at offset 72 sectors
        Checksum : 78142ae3 - correct
        Events : 17


        Device Role : Active device 0
        Array State : AA ('A' == active, '.' == missing, 'R' == replacing)
```

Now we add a HotSpare
---------------------

### What is a hotspare?
We can define a spare device that can move in and take the place of a faulty drive.
That way, you can save all of the Cat pictures for more time yoth!

```sh
[root@fatdadd disks]# ls
    hdd1  hdd2  hdd3  hdd4  hdd5
```

so lets add that fifth disk in here

```sh

[root@fatdadd disks]# mdadm /dev/md0 --add ./hdd5
    mdadm: ./hdd5 not large enough to join array
```

Oh thats right we have to use `/dev/loop5`...obviously...

```sh

[root@fatdadd disks]# mdadm /dev/md0 --add /dev/loop5
    mdadm: /dev/loop5 not large enough to join array
```

Whaaa?

```sh

[root@fatdadd disks]# du -h .
    1.1G    .
```

... But, oh wait....oh.....
If we want to add a spare, we need an equivalent sized device.
Which means we need to create a third and identical stripe.

```sh
[root@fatdadd disks]# ls -lh
    total 1.1G
    -rw-r--r-- 1 root root 1000M Feb  2 20:44 hdd1
    -rw-r--r-- 1 root root 1000M Feb  2 20:44 hdd2
    -rw-r--r-- 1 root root 1000M Feb  2 20:44 hdd3
    -rw-r--r-- 1 root root 1000M Feb  2 20:44 hdd4
    -rw-r--r-- 1 root root 1000M Feb  2 20:40 hdd5

```


```sh
[root@fatdadd disks]# cat /proc/md0
    cat: /proc/md0: No such file or directory
    [root@fatdadd disks]# cat /proc/mdstat
    Personalities : [raid1] [raid0]
    md0 : active raid1 md2[1] md1[0]
    2044928 blocks super 1.2 [2/2] [UU]

    md2 : active raid0 loop4[1] loop3[0]
    2045952 blocks super 1.2 512k chunks

    md1 : active raid0 loop2[1] loop1[0]
    2045952 blocks super 1.2 512k chunks

    unused devices: <none>

```

Level 4) Well, fine, then let's make a hot-spare the hard way.
-----------------------------------------------------

Spin up a gig file with `dd`

```sh
[root@fatdadd disks]# dd if=/dev/zero of=./hdd6 bs=1M count=1000
    1000+0 records in
    1000+0 records out
    1048576000 bytes (1.0 GB) copied, 0.609663 s, 1.7 GB/s
```

See! it's there as hdd6!

```sh
[root@fatdadd disks]# ls
    hdd1  hdd2  hdd3  hdd4  hdd5  hdd6
```

Okay now we do that thing where we mount the file as a loopback device.
Let's do that with `losetup`

```sh
[root@fatdadd disks]# losetup /dev/loop6 ./hdd6
```

Verified, `losetup` has confirmed that we have loopback

```sh
[root@fatdadd disks]# losetup
    NAME       SIZELIMIT OFFSET AUTOCLEAR RO BACK-FILE
    /dev/loop1         0      0         0  0 /root/disks/hdd1
    /dev/loop2         0      0         0  0 /root/disks/hdd2
    /dev/loop3         0      0         0  0 /root/disks/hdd3
    /dev/loop4         0      0         0  0 /root/disks/hdd4
    /dev/loop5         0      0         0  0 /root/disks/hdd5
    /dev/loop6         0      0         0  0 /root/disks/hdd6
```

Okay, now let's make that spare we've been so excited to make.
we're going to stripe disk5 and disk6 so that they we can use them as a spare in our current RAID 10

```sh

[root@fatdadd disks]# mdadm --create /dev/md3 --level=stripe --raid-devices=2 /dev/loop{5,6}
    mdadm: /dev/loop5 appears to be part of a raid array:
    level=raid1 devices=2 ctime=Sun Feb  2 20:40:27 2014
    Continue creating array? y
    mdadm: Defaulting to version 1.2 metadata
    mdadm: array /dev/md3 started.

```

Whoa, look at all of the RAID

```sh
[root@fatdadd disks]# cat /proc/mdstat
    Personalities : [raid1] [raid0]
    md3 : active raid0 loop6[1] loop5[0]
    2045952 blocks super 1.2 512k chunks

    md0 : active raid1 md2[1] md1[0]
    2044928 blocks super 1.2 [2/2] [UU]

    md2 : active raid0 loop4[1] loop3[0]
    2045952 blocks super 1.2 512k chunks

    md1 : active raid0 loop2[1] loop1[0]
    2045952 blocks super 1.2 512k chunks

    unused devices: <none>
```

Okay, Now has come the time, we are now going to add the new `/dev/md3` as a hotspare.
That way, if any of the current disks fail, we can swap this bad boy in and maintain data redundancy.

```sh
[root@fatdadd disks]# mdadm /dev/md0 --add /dev/md3
    mdadm: added /dev/md3
```

And then we of course look at the details, which show our `/dev/md3` added as a spare to the already running `/dev/md0`.
YEEEEEEEEEEEE!!!!
Pat yourself on the back. You did a great thing back there.

```sh
[root@fatdadd disks]# mdadm --detail /dev/md0
    /dev/md0:
    Version : 1.2
    Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
    Used Dev Size : 2044928 (1997.34 MiB 2094.01 MB)
        Raid Devices : 2
        Total Devices : 3
        Persistence : Superblock is persistent

        Update Time : Sun Feb  2 20:49:24 2014
        State : clean
        Active Devices : 2
        Working Devices : 3
        Failed Devices : 0
        Spare Devices : 1

    Name : fatdadd:0  (local to host fatdadd)
        UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
        Events : 18

        Number   Major   Minor   RaidDevice State
        0       9        1        0      active sync   /dev/md1
        1       9        2        1      active sync   /dev/md2

        2       9        3        -      spare   /dev/md3
```

Now break out the beers. Lay back. You have earned it for your foresight in making a redundant data system.

Nothing can stop you know

You're a legend

The only thing that could stop you at this point...

well...

you know but...

well it wont happen to me...

at least not for a while...

...

......right?

...

...

...Wrong.


```sh
[root@fatdadd disks]# mdadm /dev/md0 --fail /dev/md1
    mdadm: set /dev/md1 faulty in /dev/md0
```
.....NOOOOOOOOO!!!!!!!!!

Our Raid just  *FAILED*!

```sh
[root@fatdadd disks]# mdadm --detail /dev/md0
    /dev/md0:
        Version : 1.2
        Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
        Used Dev Size : 2044928 (1997.34 MiB 2094.01 MB)
        Raid Devices : 2
        Total Devices : 3
        Persistence : Superblock is persistent

        Update Time : Sun Feb  2 20:51:33 2014
        State : clean, degraded, recovering
        Active Devices : 1
        Working Devices : 2
        Failed Devices : 1
        Spare Devices : 1

        Rebuild Status : 87% complete

        Name : fatdadd:0  (local to host fatdadd)
        UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
        Events : 34

        Number   Major   Minor   RaidDevice State
        2       9        3        0      spare rebuilding   /dev/md3
        1       9        2        1      active sync   /dev/md2

        0       9        1        -      faulty   /dev/md1

```

HAHAHAHAHAHA!!!! you shout.

"Nice try, entropy and universal chaos," you boast, "but I had a hot-spare!"


```sh
[root@fatdadd disks]# cat /proc/mdstat
    Personalities : [raid1] [raid0]
    md3 : active raid0 loop6[1] loop5[0]
    2045952 blocks super 1.2 512k chunks

    md0 : active raid1 md3[2] md2[1] md1[0](F)
    2044928 blocks super 1.2 [2/2] [UU]

    md2 : active raid0 loop4[1] loop3[0]
    2045952 blocks super 1.2 512k chunks

    md1 : active raid0 loop2[1] loop1[0]
    2045952 blocks super 1.2 512k chunks

    unused devices: <none>
```

Check that out though.
And now its rebuilt!

```sh
[root@fatdadd disks]# mdadm --detail /dev/md0
    /dev/md0:
        Version : 1.2
        Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
        Used Dev Size : 2044928 (1997.34 MiB 2094.01 MB)
        Raid Devices : 2
        Total Devices : 3
        Persistence : Superblock is persistent

        Update Time : Sun Feb  2 20:51:36 2014
        State : clean
        Active Devices : 2
        Working Devices : 2
        Failed Devices : 1
        Spare Devices : 0

        Name : fatdadd:0  (local to host fatdadd)
        UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
        Events : 37

        Number   Major   Minor   RaidDevice State
        2       9        3        0      active sync   /dev/md3
        1       9        2        1      active sync   /dev/md2

        0       9        1        -      faulty   /dev/md1
```

And there you are, easy, your data starts failing and you already had provisions ready to spare.
And of course if you were the keen, efficient, and cunning operator that you were to do the above,
then you must be the guy that would then rush down and replace the faulty drive with a brand new upgrade.

That way you could re-add the now fixed device back into the RAID.

```sh
[root@fatdadd disks]# mdadm --manage /dev/md0 --re-add faulty
    mdadm: added 9:1
```

And now you can see, we repaired the device and go it working again in the RAID

```sh
[root@fatdadd disks]# mdadm --detail /dev/md0
    /dev/md0:
        Version : 1.2
        Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
        Used Dev Size : 2044928 (1997.34 MiB 2094.01 MB)
        Raid Devices : 2
        Total Devices : 3
        Persistence : Superblock is persistent

        Update Time : Sun Feb  2 20:53:11 2014
        State : clean
        Active Devices : 2
        Working Devices : 3
        Failed Devices : 0
        Spare Devices : 1

        Name : fatdadd:0  (local to host fatdadd)
        UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
        Events : 39

        Number   Major   Minor   RaidDevice State
        2       9        3        0      active sync   /dev/md3
        1       9        2        1      active sync   /dev/md2

        0       9        1        -      spare   /dev/md1
```

BUT THEN...
All the sudden the other disk fails!

```sh
[root@fatdadd disks]# mdadm /dev/md0 --fail /dev/md2
    mdadm: set /dev/md2 faulty in /dev/md0
```

...yeah were going through this again, the spare just steps in like "Ill save the DATA!!!"

```sh
[root@fatdadd disks]# mdadm --detail /dev/md0
    /dev/md0:
        Version : 1.2
        Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
        Used Dev Size : 2044928 (1997.34 MiB 2094.01 MB)
        Raid Devices : 2
        Total Devices : 3
        Persistence : Superblock is persistent

        Update Time : Sun Feb  2 20:53:51 2014
        State : clean
        Active Devices : 2
        Working Devices : 2
        Failed Devices : 1
        Spare Devices : 0

        Name : fatdadd:0  (local to host fatdadd)
        UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
        Events : 58

        Number   Major   Minor   RaidDevice State
        2       9        3        0      active sync   /dev/md3
        0       9        1        1      active sync   /dev/md1

        1       9        2        -      faulty   /dev/md2
```

then you fix it and ...blah..blah..blah..

```sh
[root@fatdadd disks]# mdadm --manage /dev/md0 --re-add faulty
    mdadm: added 9:2
```

Yes there it is..can we move on?

```sh
[root@fatdadd disks]# mdadm --detail /dev/md0
    /dev/md0:
        Version : 1.2
        Creation Time : Sun Feb  2 20:43:42 2014
        Raid Level : raid1
        Array Size : 2044928 (1997.34 MiB 2094.01 MB)
        Used Dev Size : 2044928 (1997.34 MiB 2094.01 MB)
        Raid Devices : 2
        Total Devices : 3
        Persistence : Superblock is persistent

        Update Time : Sun Feb  2 20:54:12 2014
        State : clean
        Active Devices : 2
        Working Devices : 3
        Failed Devices : 0
        Spare Devices : 1

        Name : fatdadd:0  (local to host fatdadd)
        UUID : e6c5f1fe:3d804297:8c661fce:e7f5c9a1
        Events : 60

        Number   Major   Minor   RaidDevice State
        2       9        3        0      active sync   /dev/md3
        0       9        1        1      active sync   /dev/md1

        1       9        2        -      spare   /dev/md2
        [jj]
        </none>
```

No, cause I have no more material
