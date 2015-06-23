ZFS lab Hardmode
================

So you decided you want a bit of a challenge...

What is ZFS?
------------

ZFS is a filesystem primarily used by Solaris and Solaris derivatives. It makes an excellent fileserver filesystem with features like snapshots, error-checking, speed, and all sorts of other super cool things. ZFS raid is done through software above the hardware and disks.

###On with the show!

The first thing you probably want to do is `man zfs` and `man zpool` for creating zfs filesystems and zfs storage pools, respectively. Also, check out the [ZFS Best Practices from Oracle][ZFS]

1. Create a mirrored zpool with the disk ids
2. find where the mirrored zpool you just created got mounted on the filesystem
3. check the status of your pool
4. destroy your mirrored zpool
5. create a raidz2 pool
6. check the status of your raidz2 pool
7. destroy the pool 
8. create a raidz2 pool with a cache disk a spare and a log device
9. say you have a failing disk, replace a failing disk with a spare
10. check the status of your rpool after the failure 
11. create a zfs filesystem and configure it to be shared over nfs

[ZFS]:http://docs.oracle.com/cd/E23824_01/html/E24456/storage-4.html
