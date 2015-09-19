/*
 * Copyright 2015 Sergey Kvachonok
 *
 * Based in part on utils.c from btrfs-progs which is
 * Copyright (C) 2007 Oracle.  All rights reserved.
 * Copyright (C) 2008 Morey Roof.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/btrfs.h>

#include <config.h>

/* BTRFS control device for the device scan ioctls */
static char btrfs_control_node[] = "/dev/btrfs-control";

static int btrfs_open_control_node(const char *fname)
{
	int fd;

	fd = open(fname, O_RDWR);
	if (fd < 0) {
		fprintf(stderr,
			"ERROR: Failed to open control device '%s': %s\n",
			fname, strerror(errno));
		return -1;
	}

	return fd;
}

static int btrfs_register_one_device(int ctrl_fd, const char *devname)
{
	static struct btrfs_ioctl_vol_args args;
	int ret;

	memset(&args, 0, sizeof(args));
	strncpy(args.name, devname, sizeof(args.name) - 1);

	ret = ioctl(ctrl_fd, BTRFS_IOC_SCAN_DEV, &args);
	if (ret < 0) {
		fprintf(stderr,
			"WARNING: Registering device '%s' failed: %s\n",
			devname, strerror(errno));
	}

	return ret;
}

static void print_usage(void)
{
	puts("Usage: btrfs-register DEVICE [DEVICE2]...");
	puts("Register block device(s) with the kernel BTRFS driver.");
	puts("");
        puts("  --help     display this help and exit");
        puts("  --version  output version information and exit");
	puts("");
	puts("It is only needed for mounting multiple device "
	     "BTRFS root filesystems.");
}

static void print_version(void)
{
	puts(PACKAGE_NAME " " PACKAGE_VERSION);
	puts("Copyright 2015 Sergey Kvachonok and others");
	puts("License GPLv2: GNU GPL version 2");
}

int main(int argc, char* argv[])
{
	int i;
	int nfailed;

	int ctrl_fd;

	if (argc < 2) {
		print_usage();
		return 0;
	}

	if (!strncmp(argv[1], "--help", 6)) {
		print_usage();
		return 0;
	}

	if (!strncmp(argv[1], "--version", 9)) {
		print_version();
		return 0;
	}

	if (argv[1][0] == '-') {
		fprintf(stderr, "ERROR: Unrecognized option '%s'\n", argv[1]);
		fprintf(stderr, "Try 'btrfs-register --help' for usage\n");
		return -1;
	}

	ctrl_fd = btrfs_open_control_node(btrfs_control_node);
	if (ctrl_fd < 0)
		return -2;

	nfailed = 0;

	for (i = 1; i < argc; i++) {
		int ret;

		ret = btrfs_register_one_device(ctrl_fd, argv[i]);

		if (ret < 0)
			nfailed++;
	}

	if (nfailed) {
		fprintf(stderr,
			"WARNING: %d out of %d devices failed to register\n",
			nfailed, argc - 1);
	}

	return 0;
}
