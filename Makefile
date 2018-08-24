CROSS_COMPILE ?= x86_64-unknown-linux-gnu-
export CC := $(CROSS_COMPILE)gcc
export CXX := $(CROSS_COMPILE)g++
export ROOT_DIR := $(shell pwd)
export BUILD_DIR := $(ROOT_DIR)/Build/gen
export BIN_DIR := $(ROOT_DIR)/Build/ARIX
export MAKE := make
export INCLUDE_DIR := $(shell pwd)/include
export CFLAGS := -Os -I$(INCLUDE_DIR)

SUBDIRS := libs

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
