ZFS lab Intermediate Edition
================

So you decided you want a bit of a challenge, but still would like some guidance. You have come to the right place.

What is ZFS?
------------

ZFS is a filesystem primarily used by Solaris and Solaris derivatives. It makes an excellent fileserver filesystem with features like snapshots, error-checking, speed, and all sorts of other super cool things. ZFS raid is done through software above the hardware and disks.

###On with the lab!

The first thing you probably want to do is `man zfs` and `man zpool` for creating zfs filesystems and zfs storage pools, respectively. Also, check out the [ZFS Best Practices from Oracle][ZFS]

1. Create a mirrored zpool with the disk ids
   * Need three things: pool name, pooling strategy, and list of devices to pool
        * Name: Name of your pool
        * Pool Strategy: how you want to arrange your devices together (RAID types, physical organization)
        * Devices: real devices (like a hard drive, or SSD) or pseudo devices (like loopback files)
  *HINT: `man zpool` and search for create, see how disks are represented (/dev/disk)

2. find where the mirrored zpool you just created got mounted on the filesystem
  *HINT: poke around where system directories are mounted, such as etc var usr home

3. check the status of your pool
  *HINT: `man zpool`

4. destroy your mirrored zpool
  *HINT: `man zpool`

5. create a raidz2 pool
   *HINT: just like mirrored, just with a different pooling strategy

6. check the status of your raidz2 pool

7. destroy the pool 

8. create a raidz2 pool with a cache disk, a spare, and log disk.
   So a cache disk is a device where disk I/O can be cached while it is written to the disk array. A spare is exactly what it sounds like, a standby device / disk that can be put into play should one of the disks fail. A log disk is a separate device that acts as a ZIL (ZFS Intent Log) to speed up performance for writes. This is usually done by allocating blocks on each device for intent logging. If you had a SSH you could create a standalone log device to speed up I/O performance.
   *HINT: similar to step 5, but check out the cache, log, and spare keywords

9. say you have a 'failing' disk, replace a failing disk with a spare
   *HINT: replace keyword

10. check the status of your rpool after the failure 

11. create a zfs filesystem in your pool and configure it to be shared over NFS
   *HINT: man zfs, look for nfs

[ZFS]:http://docs.oracle.com/cd/E23824_01/html/E24456/storage-4.html
