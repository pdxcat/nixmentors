ZFS
===

A wise man once said:

"ZFS? I hardly knew her..."

You all today will become greater than that man.


What is ZFS?
------------

ZFS is like the nuclear cannon of Solaris. It is their Ace of
spaces and it is the reason that the system is arguably the best
choice for high load fileserving. The best way I can describe it
is like...


### Say you had a Burrito...

![I'm a Burrito!](http://fc09.deviantart.net/fs70/f/2013/051/4/8/burrito_cat_para_estetio_by_frikitty-d5vmyzj.png)

...filled with tomatoes, and chilies, and guacamole, and hollandaise sauce,
and just about all of the other great burrito ingredients that you should
so choose to decide to throw at it. The wrap is always big enough.
And also say that this burrito had super powers, that could allow it
to be in two places at once, as if it was in a mirror room at a carnival
or a bad secret agent movie. Lets also suppose that you could take three
bites of the burrito at once! Getting three times the juicy innards every
mouth globbering go around. This burrito also shares itself with your friends
and family, and emails you when it gets sick. The burrito is also very
intelligent, and can even replace stales beans with magical hot spare beans
that await their turn to become a part of this magical experience.

### Say What?

Okay sure, I knew that analogy was kind of a stretch, and most likely
was a sad attempt to describe this incredible technology all wrapped (hehe)
into one golden, warm, marriage killing toilet destroyer.

### Let's try this again...

Go with me on this one. Say you wanted to create the worlds greatest
fileserver. You would most certainly need a few things to ensure
your supremacy.

* Redundancy, either by [Mirroring][mirroring] providing [Parity][Parity]
* Speed, most likely from [Striping][Striping]
* Backups/snapshots
* All sorts of cool features (not limited to)
    * [NFS][NFS]
    * [Samba][Samba]
    * [Access Control][AccessControl]
    * [Disk Quota][DiskQuota]
    * [Hot Spares][HotSpares]
    * [Etc][ZFSProperties]


### Are we there yet?

*NO* and if you ask me one more gosh darn time I'm going to turn this paper
around and teach you how to print double sided text files with `lpr`...

...Let's just continue.

The Dichotomy
-------------

![Anther Great Duo](http://fc03.deviantart.net/fs70/i/2013/131/8/9/batman_and_robin_by_cute_aholic-d64uxn1.jpg)

There are two integral parts to this magical experience. You need to create the
container for the ZFS filesystem. For this we will use a command called `zpool`.
Secondly, will will use a command called `ZFS` to make sub-systems within that
container.

### Just nod you heads for right now, I'll go into it

Zpool is first up. What `zpool` will do is take the ingredients of our burrito,
err... it will take the disks we want to make availible and wrap them all up into
one nice manageable system. Now, we can do this in any number of ways. We aren't
exactly limited to just burritos here. We can make tacos, chimichangas, tostadas,
enchuritos, XXL Grilled Stuffed burritos(TM), and the faithful homemade, burrito.
Saying that again in ZFS terms, we could wrap our disks up in mirrors, stripes,
striped mirrors, mirrored stripes, stripes with parity, mirrored stripes with
multiple parity drives with extra hotspares, the combinations are nearly endless.
And like the burrito each different style of disk folding serves its own unique
purpose.

### Let's Create one already!

okay, okay, hold your horses there cow{boy,girl}, I'm just having a little fun.
When using `zpool` you have to understand how it takes its arguments. Thankfully
its pretty straight forward and consistent in its style. Here is an example:

`root@sunosfiler1:~# zpool create mpool mirror c9t1d0 c9t2d0 c9t3d0`

Alright, we have a couple things going on here. Let's get this over with.

* `root@sunosfiler1:~#` is my shell prompt, its at the begining of all of my commands
* `zpool` is the name of the command, (if you didn't get this part, quietly leave the room)
* `create` is the action we want to take, and in this case, will create a pool
* `mpool` is the name of the pool we are referring to, in case of create, this better be new
* `mirror` is the type if pool that we are aiming to create, in this case, mirror the disks
* `c9t1d0 c9t2d0 c9t3d0` are the names of the disks that we want to include into our pool

After executing the command will should see no output, which is a great thing. It means
that the command executed successfully and now we will have a newly created filesystem 
that will live, by default in the root of the whole \*nix filesystem. We can see this
by listing the contents of root.

```
root@sunosfiler1:~# ls /
bin       dev       export    lib       net       platform  rpool     tmp
boot      devices   home      media     nfs4      proc      sbin      usr
cdrom     etc       kernel    mnt       opt       root      system    var
```
Before I go on much further, it is also very worth mentioning that there is a very
handy command to check the status of a pool. `zpool status <PoolName>`

```
root@sunosfiler1:~# zpool status mpool
  pool: mpool
 state: ONLINE
  scan: none requested
config:

   NAME         STATE     READ WRITE CKSUM
   mpool        ONLINE       0     0     0
     mirror-0   ONLINE       0     0     0
       c9t1d0   ONLINE       0     0     0
       c9t2d0   ONLINE       0     0     0
       c9t3d0   ONLINE       0     0     0

   errors: No known data errors
```
Which tells us about the pool, notably that it is in fact `ONLINE` and that the
three disks we added to the pool are in fact in a `mirror-0` RAID configuration.

### Proceed to pat yourself on the back

And there you have it. You made a zpool. Good job. I can see that a {guy,girl}
like you is definitely going places. Please take this time to catch your breath,
eat a snickers, and/or call your mom, because you {sir,madam} have earned it.

### Okay, sit back down, shut up, and let's move on

Now just like my father once said:

`INSERT_NAME... I brought you into this world, and I can sure as S*** take you out`

![And he meant it](http://www.buzzreactor.com/sites/default/files/imagecache/picture/images/articles/clint_eastwood.jpg)

and with those words of wisdom, lets take out our pool. This is done with a handy
command called `zpool destroy <PoolName>`. Lettuce try this shall we, by taking out
the pool we so painstakenly worked our butt off to nurture and to hold and to take
to those darn ukelele lessons...

`root@sunosfiler1:~# zpool destroy mpool`

... wait that was it? Our pool..you mean to tell me it just up and vanished just like
that? Yes. It did. Remember young padawan, this is \*nix, which means no news is good
news.

```
root@sunosfiler1:~# zpool status mpool
cannot open 'mpool': no such pool
```

see? that sucker does not even exist for a bit, and now we are free to re use its
disks to create even more pools. Hopefully some more pools that will work right,
appretiate the hard work we do and play a real instrument like the oboe...

...by the way, another handy command here is `zpool list`. This handy piece of
wisdom will tell us about all the current pools in operation, plus some bonus
information like, how big it is, how mush is in use, how much is free, its health
and some other stuff i haven't read about yet.

```
root@sunosfiler1:~# zpool list
NAME    SIZE  ALLOC   FREE  CAP  DEDUP  HEALTH  ALTROOT
rpool  7.69G  3.69G  4.00G  48%  1.00x  ONLINE  -
```

Pretty cool huh?
)
### LEVEL UP!

![Get some!](https://lh3.ggpht.com/-l7K-8C5VDfQ/T9bfQLwQCGI/AAAAAAAAAt4/8NunUrza5Xk/s1600/levelup-440x300.jpg)

Woh! Nice, we just leveled up here. We made a zpool and even destroyed it and most
importantly, issued a few zpool commands, thus arranging our neurons to better understand
the steps that are about to come. You are no longer inner tubing on the bunny slopes my friend.
You are about to be skiing on you own (with training wheels and an ambulance on call of
course) down the Matterhorn at a break-neck pace here.

![Good luck](http://thumbs.imagekind.com/1571596_650/Matterhorn.jpg)

### Lets make some enchuritos!

You remember that time... when we mirrored those disks together? That was cool and
all, but my friends told me about this thing called striping and parity, and they
made it sound all cool. So, lets like, hop on the bandwagon shall we?

You see, the `man` page for zpool has some really good explanation as to all the
different pools you can make. We have at our disposal things like:

#### mirror
mirrors are used to creates a pool focused on preserving data in a redundant manner.
basically we copy data to multiple disks, so that if one goes down, we still have the
information stored on another.

#### raidz[123]?
Sorry about that regex there, but I couldn't help myself. raidz stripes disks, and
provides multiple levels of parity (denoted by the trailing digit) that ensure a bit
of safety in case a disk goes down. Lemme expound upon this a bit. When we stripe drives
we save a block of data in pieces over each of the drives. So a parity drive, takes each
piece and calculates a value that can be used later to recover the information if a drive
goes down. Back in the day we would dedicate an entire drive to this, but now we prefer
to distribute the parity over all the drives. (Ask me later if this doesn't make sense, or
if it looks like I'm bored, ask me now!)

#### cache
mirror and raidz are very common, but it also helps in certain situations to have a cache
drive. Don't bog yourself down too hard trying to understand this part as we don't use this
all that often but it gives us a system that will cache information about ZFS stuffs, and
in theory make things happen faster.

#### log
As with every great service, there is a level of logging that is necessary. In ZFS you can
dedicate a drive for this purpose. It'll log the heck out of your ZFS system, which is really
handy to have in some situations.

### Examples you say!? "Sure" I exclaim

Let's just jump straight to the point and use all of the new features we just went over. This
is going to be a blast.

```
root@sunosfiler1:~# zpool create mypool raidz2 c9t1d0 c9t2d0 c9t3d0 c9t4d0 cache c9t5d0 log c9t6d0
```

I'm going to go over this...again:

* `zpool` is the command. duh...
* `create` is the action, of course...
* `mypool` is the name, you know this...
* `raidz2 c9t1d0 c9t2d0 c9t3d0 c9t4d0` is going to create a striped RAID with two levels of parity
* `cache c9t5d0` is going to allocate a drive to serve as a caching mechanism
* `log c9t6d0` is going to log all of the things

Let's see what it ended up looking like:

```
root@sunosfiler1:~# zpool status mypool
  pool: mypool
 state: ONLINE
  scan: none requested
config:

        NAME        STATE     READ WRITE CKSUM
        mypool      ONLINE       0     0     0
          raidz2-0  ONLINE       0     0     0
            c9t1d0  ONLINE       0     0     0
            c9t2d0  ONLINE       0     0     0
            c9t3d0  ONLINE       0     0     0
            c9t4d0  ONLINE       0     0     0
        logs
          c9t6d0    ONLINE       0     0     0
        cache
          c9t5d0    ONLINE       0     0     0

errors: No known data errors
```

Okay we can see our drives all put together like. Our drives `c9t1d0 c9t2d0 c9t3d0 c9t4d0`
became the base for the raidz2. `c9t6d0` is going to be used to manage the logging for the
pool. And Finally `c9t5d0` is going to give us a cache drive to ensure that we have enough
space to keep track of all crazy stats and references that we need.

If we look at the pool's listing using `zpool list <PoolName>` we can see how much space we
are able to use, and such and such and so forth...

```
root@sunosfiler1:~# zpool list mypool
NAME     SIZE  ALLOC   FREE  CAP  DEDUP  HEALTH  ALTROOT
mypool  3.94G   267K  3.94G   0%  1.00x  ONLINE  -
```

### What!? Another sweet example traveling by way of limo!?

![OH Baller Limo Riding](http://0.s3.envato.com/files/29315893/SUV_limo%20_pv.jpg)

Sure, but not without introducing another cool feature. Hot Spares. Remember those beans that
were getting all toasty for their chance to become a part of our burrito? Well, what ZFS does
with hot spares is exactly (give or take an analogy or two) like that. We are going to save a
couple disks on the side in case of emergency. If we just so happen to lose a disk during the
life of our pool, one of these hot spares will be used to take its place. It really is quite
a heartwarming tale. Let's watch!

First, let's revisit our command with a little bit of extra sour cream (I KNOW IT'S EXTRA!!!)
added on top.
```
root@sunosfiler1:~# zpool create mypool raidz2 c9t1d0 c9t2d0 c9t3d0 c9t4d0 cache c9t5d0 log c9t6d0 spare c9t7d0 c9t8d0
```

All we did was add the `spare` clause at the end, followed by the disks `c9t7d0 c9t8d0` that
we wanted chill on the bench and wait to be called in like a that scrawny kid that no one ever
thought would end up bunting the ball that would allow the runner on third to come in and score
the winning run in the state championship game.

We can see those kids just sitting there, again with `zpool status <PoolName>`

```
root@sunosfiler1:~# zpool status mypool
  pool: mypool
 state: ONLINE
  scan: none requested
config:

        NAME        STATE     READ WRITE CKSUM
        mypool      ONLINE       0     0     0
          raidz2-0  ONLINE       0     0     0
            c9t1d0  ONLINE       0     0     0
            c9t2d0  ONLINE       0     0     0
            c9t3d0  ONLINE       0     0     0
            c9t4d0  ONLINE       0     0     0
        logs
          c9t6d0    ONLINE       0     0     0
        cache
          c9t5d0    ONLINE       0     0     0
        spares
          c9t7d0    AVAIL   
          c9t8d0    AVAIL   

errors: No known data errors
```

Yup, and there they are, did our pool change much in size?

```
root@sunosfiler1:~# zpool list mypool
NAME     SIZE  ALLOC   FREE  CAP  DEDUP  HEALTH  ALTROOT
mypool  3.94G   285K  3.94G   0%  1.00x  ONLINE  -
```

Nope, but that is fine, I don't think we really should have expected it to.

### What did you say lassie? c9t1d0 is failing!?

![And Lassie then proceeded to text her buttocks off](http://www.davidrdudley.com/davidrdudley.com/Lassie_texting_files/lassie_Dudley_forWeb.jpg)

Well I mean, really it was inevitable right? On eof the disks was surely ready to fail
now we got to sstep in and save the pool. Easy, because we have 
`zpool replace <PoolName> <FailDisk> <SpareDisk>` to come and rescue us proper.

```
root@sunosfiler1:~# zpool replace mypool c9t1d0 c9t7d0
```

And now we can see the spare step in and save the day, taking the place of the poor
failing disk and holding it up like a Vietnam soldier intent on getting is comrade
home in hopes that the guys wife makes a dank apple pie.

```
root@sunosfiler1:~# zpool status mypool
  pool: mypool
 state: ONLINE
  scan: resilvered 58.5K in 0h0m with 0 errors on Thu May  1 06:05:03 2014
config:

        NAME          STATE     READ WRITE CKSUM
        mypool        ONLINE       0     0     0
          raidz2-0    ONLINE       0     0     0
            spare-0   ONLINE       0     0     0
              c9t1d0  ONLINE       0     0     0
              c9t7d0  ONLINE       0     0     0
            c9t2d0    ONLINE       0     0     0
            c9t3d0    ONLINE       0     0     0
            c9t4d0    ONLINE       0     0     0
        logs
          c9t6d0      ONLINE       0     0     0
        cache
          c9t5d0      ONLINE       0     0     0
        spares
          c9t7d0      INUSE   
          c9t8d0      AVAIL   

errors: No known data errors
```
We can see now that the spare is `INUSE` and that it is aiding the poor disk in
combat.


#### Enable zfs snapshots

Now for something completely different...

Since ZFS is a copy-on-write file-system, we can use something called snapshoting.
A snapshot is a read-only version of a file-system or volume at a given point in time.
Snapshots can be made almost instantly and consume a trivial amount of space in the zpool.
However, the amount of space a dataset occupies will increase as the data within an active dataset changes.

(For extra fun, investigate zfs snapshot clones.)

Lets try first creating a dataset (aka file-system) and then a snapshot of it:
```
root@sunosfiler1:~# zfs create mypool/dataset1
root@sunosfiler1:~# zfs snapshot mypool/dataset1@snap1
root@sunosfiler1:~# zfs list
root@sunosfiler1:~# zfs list -t snapshot
```
This creates a dataset called 'dataset1' then creates a snapshot of it called 'dataset1@snap1'.
We then list all the zfs file-systems, and the zfs snapshots.
Notice that the snapshots don't show-up in the list of zfs file-systems.


#### Setting a quota on a dataset

Now that we have a dataset created, suppose that we'd like to limit the amount space in the pool
which it can occupy. We can do this by applying a quota to it.

To set a quota of 5GB on 'dataset1', try this:
```
root@sunosfiler1:~# zfs set quota=5G mypool/dataset1
root@sunosfiler1:~# zfs get all mypool/dataset1 | grep quota
```

Notice that the last command shows us that there is a 5GB quota on 'dataset1' as we set, but also
that there is a property called refquota which has value 'none'.
A quota is a hard-limit on a dataset which included all snapshots and sub-file-systems (aka decendents) of it.
A refquota is a hard-limit that DOES NOT include snapshots and decendents.






[Mirroring]: http://en.wikipedia.org/wiki/Mirroring_disks
[Parity]: http://en.wikipedia.org/wiki/Parity_drive
[Striping]: http://en.wikipedia.org/wiki/Data_striping
[NFS]: http://en.wikipedia.org/wiki/Network_File_System
[Samba]: http://en.wikipedia.org/wiki/Samba
[AccessControl]: http://en.wikipedia.org/wiki/Access_control_list
[DiskQuota]: http://en.wikipedia.org/wiki/Disk_quota
[HotSpares]: http://en.wikipedia.org/wiki/Hot_spare
[ZFSProperties]: http://docs.oracle.com/cd/E19253-01/819-5461/gazss/index.html


