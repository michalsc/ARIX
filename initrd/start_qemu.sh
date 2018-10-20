#!/bin/sh

qemu-system-x86_64 -kernel ../Build-x86_64/vmlinuz -initrd ../Build-x86_64/initrd -m 1024 -nographic -append "console=ttyS0 loglevel=7"
