export ROOT_DIR := $(shell pwd)

-include $(ROOT_DIR)/config/user_config.mk
include $(ROOT_DIR)/config/version.mk

export ARCH
export CC := $(CROSS_COMPILE)gcc
export CXX := $(CROSS_COMPILE)g++
export BUILD_DIR := $(ROOT_DIR)/Build/gen
export BIN_DIR := $(ROOT_DIR)/Build/ARIX
export MAKE := make -s
export INCLUDE_DIR := $(BIN_DIR)/Development/include
export CFLAGS := -Os -std=c11 -I$(INCLUDE_DIR) $(USER_CFLAGS)
export CXXFLAGS := -Os -std=c++11 -I$(INCLUDE_DIR) $(USER_CXXFLAGS)
export LDFLAGS := $(USER_LDFLAGS)
export SFDC := $(ROOT_DIR)/tools/sfdc/sfdc.py

SUBDIRS := libs system test

all: includes $(SUBDIRS)

initrd: includes $(SUBDIRS)
	@echo "Building the initial ramdisk"
	@cp $(ROOT_DIR)/initrd/vmlinuz-$(ARCH) $(ROOT_DIR)/Build
	@cp $(ROOT_DIR)/initrd/arix_base_cpio-$(ARCH).img.gz $(ROOT_DIR)/Build/initrd
	@cd $(ROOT_DIR)/Build && find ARIX | cpio -o -H newc 2>/dev/null | gzip >> $(ROOT_DIR)/Build/initrd

includes:
	@echo "Generating header files"
	@$(MAKE) -C include includes
	@$(MAKE) -C libs includes

$(SUBDIRS):
	@$(MAKE) -C $@ all

.PHONY: all includes initrd $(SUBDIRS)
