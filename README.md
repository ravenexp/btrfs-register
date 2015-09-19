# btrfs-register

A minimal statically linked 'btrfs device scan' replacement for busybox based
initramfs builds.

btrfs-progs has too many dependencies to be easily statically compiled.

Example initramfs script usage:

    echo "Waiting for devices..."
    sleep 1

    echo "Registering BTRFS block devices"
    echo "Root partition candidates:" /dev/sd[a-f]2
    /bin/btrfs-register /dev/sd[a-f]2

...then proceed to mount the root filesystem.

