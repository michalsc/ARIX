#!/bin/sh

qemu-system-x86_64 -kernel ../Build/vmlinuz -initrd ../Build/initrd -m 512 -nographic -append "console=ttyS0 loglevel=7"
