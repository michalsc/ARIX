#!/bin/sh

qemu-system-arm -M virt -m 1024 -kernel ../Build-armhf/vmlinuz -initrd ../Build-armhf/initrd -nographic -no-reboot
