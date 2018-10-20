#!/bin/sh

qemu-system-aarch64  -machine raspi3 -kernel ../Build-aarch64/vmlinuz -initrd ../Build-aarch64/initrd -m 1024  -append "earlycon=pl011,0x3f201000 console=ttyAMA0 loglevel=6" -nographic -dtb bcm2710-rpi-3-b.dtb
