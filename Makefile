-include user_config.mk

export CC := $(CROSS_COMPILE)gcc
export CXX := $(CROSS_COMPILE)g++
export ROOT_DIR := $(shell pwd)
export BUILD_DIR := $(ROOT_DIR)/Build/gen
export BIN_DIR := $(ROOT_DIR)/Build/ARIX
export MAKE := make
export INCLUDE_DIR := $(BIN_DIR)/Development/include
export CFLAGS := -Os -std=c11 -I$(INCLUDE_DIR) $(USER_CFLAGS) -fno-strict-aliasing
export CXXFLAGS := -Os -I$(INCLUDE_DIR) $(USER_CXXFLAGS) -fno-strict-aliasing
export SFDC := $(ROOT_DIR)/tools/sfdc/sfdc.py

SUBDIRS := libs test arix

all: includes $(SUBDIRS)

includes:
	@$(MAKE) -C include includes
	@$(MAKE) -C libs includes

$(SUBDIRS):
	@$(MAKE) -C $@ all

.PHONY: all includes $(SUBDIRS)
