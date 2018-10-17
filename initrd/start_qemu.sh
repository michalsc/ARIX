#!/bin/sh

qemu-system-x86_64 -serial stdio -kernel ../Build/vmlinuz -initrd ../Build/initrd -append "init=/ARIX/System/init root=ram0 console=ttyS0 loglevel=7"
