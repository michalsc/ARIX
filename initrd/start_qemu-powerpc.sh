#!/bin/sh

qemu-system-ppc -M mac99 -m 1024 -kernel ../Build-powerpc/vmlinuz -initrd ../Build-powerpc/initrd -nographic -append "console=ttyS0"
