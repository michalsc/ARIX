CROSS_COMPILE ?= x86_64-unknown-linux-gnu-
export CC := $(CROSS_COMPILE)gcc
export CXX := $(CROSS_COMPILE)g++
export ROOT_DIR := $(shell pwd)
export BUILD_DIR := $(ROOT_DIR)/Build/gen
export BIN_DIR := $(ROOT_DIR)/Build/ARIX
export MAKE := make
export INCLUDE_DIR := $(BIN_DIR)/Development/include
export CFLAGS := -Os -I$(INCLUDE_DIR)
export CXXFLAGS := -Os -I$(INCLUDE_DIR)
export SFDC := $(ROOT_DIR)/tools/sfdc/sfdc.py

SUBDIRS := libs test

all: includes $(SUBDIRS)

includes:
	@$(MAKE) -C include includes
	@$(MAKE) -C libs includes

$(SUBDIRS):
	@$(MAKE) -C $@ all

.PHONY: all includes $(SUBDIRS)
