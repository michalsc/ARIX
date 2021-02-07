OBJNAME   := kernel
OBJTYPE   := library
TARGETDIR := Libs
LDLIBS    := 
GLOBAL    := LibBase KernelBase
LOCAL     := *
LDFLAGS	  := $(LDFLAGS) -nolibc
FILES     := kernel_init sc_open sc_close sc_exit sc_write sc_read sc_mkdir sc_execve sc_mount \
			 sc_lseek sc_memfd_create sc_fcntl sc_clock_gettime sc_clock_settime sc_clock_getres \
			 sc_socket sc_bind

include $(ROOT_DIR)/config/make_shared.mk
include $(ROOT_DIR)/config/make_bottom.mk
