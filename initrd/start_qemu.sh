#!/bin/sh

qemu-system-x86_64 -serial stdio -kernel ../Build/vmlinuz -initrd ../Build/initrd -m 512 -append "console=ttyS0 loglevel=7"
