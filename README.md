# ARIX

## Introduction

ARIX is a set of libraries inspired by the AmigaOS application interface, placed on top of a linux kernel. The libraries provide basic memory management (allocation, reallocation, filling memory etc), thread and process creation, inter process communication and many more. The ARIX libraries themselves does not depend on any other high-level linux libraries apart from some very low level things, such as /lib/ld-linux.so or the dynamic/static library dependencies introduced by the gcc.

The dos library, responsible e.g. for file access or management of environment variables, provides transparent translation of Volumes, Assigns and Devices into the unix filesystem tree visible by linux kernel.

On top of these libraries there are the several tools and applications – the basic system init (replacement of /sbin/init), main ARIX process, debug process collecting, storing and eventually displaying messages send to it and many others. They resemble the AmigaOS structure and provide similar shell, giving one Amiga feeling.

The entire ARIX is aimed at simplicity, it is not intended to be the multi-user server-alike operating system.

## Current status

Right now the ARIX as a whole is in very early development stage. There are only few libraries available and these are still not ready. User space tools are missing, only the /sbin/init replacement and bits of main ARIX process are available. More things are coming, though.