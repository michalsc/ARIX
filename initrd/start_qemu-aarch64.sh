#!/bin/sh

qemu-system-aarch64 -machine virt -kernel ../Build-aarch64/vmlinuz -initrd ../Build-aarch64/initrd -m 1024 -cpu cortex-a53 -nographic -no-reboot
