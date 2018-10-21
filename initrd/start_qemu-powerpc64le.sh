#!/bin/sh

qemu-system-ppc64 -smp cores=2,threads=1 -nodefaults  -serial stdio -m 1024 -kernel ../Build-powerpc64le/vmlinuz -initrd ../Build-powerpc64le/initrd -nographic 
