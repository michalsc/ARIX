export ROOT_DIR := $(shell pwd)

-include $(ROOT_DIR)/config/user_config.mk

export CC := $(CROSS_COMPILE)gcc
export CXX := $(CROSS_COMPILE)g++
export BUILD_DIR := $(ROOT_DIR)/Build/gen
export BIN_DIR := $(ROOT_DIR)/Build/ARIX
export MAKE := make -s
export INCLUDE_DIR := $(BIN_DIR)/Development/include
export CFLAGS := -Os -std=c11 -I$(INCLUDE_DIR) $(USER_CFLAGS)
export CXXFLAGS := -Os -std=c++11 -I$(INCLUDE_DIR) $(USER_CXXFLAGS)
export SFDC := $(ROOT_DIR)/tools/sfdc/sfdc.py

SUBDIRS := libs test arix

all: includes $(SUBDIRS)

includes:
	@$(MAKE) -C include includes
	@$(MAKE) -C libs includes

$(SUBDIRS):
	@$(MAKE) -C $@ all

.PHONY: all includes $(SUBDIRS)
