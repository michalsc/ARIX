#!/bin/sh

taskset -c 4-5 qemu-system-aarch64 -machine virt -kernel ../Build-aarch64/vmlinuz -initrd ../Build-aarch64/initrd -m 1024 -cpu host -nographic -no-reboot -append "loglevel=3" -smp 4 -accel kvm
