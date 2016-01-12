#!/bin/sh
#
# config.mk
#
# 2015.09.23
# by erato.
#
#

#
# build mode
#
BUILD_MODE ?= debug
BUILD_OPTION ?= shared_lib

#
# PATH
#
LOCAL_PATH 		:= $(shell pwd)
TOP_PATH		:= $(LOCAL_PATH)/..
TOP_BUILD_PATH	:= $(TOP_PATH)/build
TOP_OUT_PATH 	:= $(TOP_PATH)/out/$(BUILD_MODE)

#
# complie option
#
AT				:= @
ARCH			:=
CC				:= gcc
CXX				:= g++
LOCAL_CDEFINES 		:=
LOCAL_CXXDEFINES 	:=
LOCAL_CFLAGS 		:= -Wall -Wno-unused-parameter -std=c99
LOCAL_CXXFLAGS 		:= -Wall -Wno-unused-parameter -std=gnu++0x 
LOCAL_CLDFLAGS		:= -L$(TOP_OUT_PATH)
LOCAL_CXXLDFLAGS	:= -L$(TOP_OUT_PATH)


ifeq ($(BUILD_MODE),debug)
 LOCAL_CFLAGS += -g
 LOCAL_CXXFLAGS += -g
else
 LOCAL_CDEFINES += -DNDEBUG
 LOCAL_CXXDEFINES += -DNDEBUG
endif

ifneq ($(ARCH),)
 CC := $(ARCH)-$(CC)
 CXX := $(ARCH)-$(CXX)
endif

BUILD := make -C


#
# export
#
export BUILD_MODE AT ARCH
export CC CXX LOCAL_CDEFINES LOCAL_CXXDEFINES LOCAL_CFLAGS LOCAL_CXXFLAGS LOCAL_CLDFLAGS LOCAL_CXXLDFLAGS
export BUILD BUILD_OPTION
export TOP_PATH TOP_BUILD_PATH TOP_OUT_PATH
