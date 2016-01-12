#!bin/sh
# Makefile
#
# 2015.09.23
# erato.
#


#
# PATH
#
LOCAL_PATH := $(shell pwd)

ifeq ($(strip $(TOP_BUILD_PATH)),)
	TOP_BUILD_PATH 	?= $(LOCAL_PATH)
	include $(TOP_BUILD_PATH)/config.mk
endif

OBJECT_PATH := $(LOCAL_PATH)/obj
OUT_PATH := $(LOCAL_PATH)/out
TARGET_PATH := $(OUT_PATH)

LOCAL_INC_PATH := $(LOCAL_PATH)/include \
				$(LOCAL_PATH)/src \
				$(LOCAL_PATH)/libdvbsi++/include/dvbsi++ 

LOCAL_SRC_PATH := $(LOCAL_PATH)/src

#
# BUILD FLAGS
#
BUILD_OPTION := shared_lib

LOCAL_MODULE := psip

LOCAL_CXXINCLUDES := \
	$(foreach path, $(LOCAL_INC_PATH),	\
		-I$(path)\
	) \
	$(foreach path, $(LOCAL_SRC_PATH),	\
		-I$(path)\
	) \

# test
LOCAL_CXXINCLUDES += \

LOCAL_SRC_FILES := \
	$(foreach path, $(LOCAL_SRC_PATH),	\
		$(wildcard $(path)/*.cpp) \
	) \
	
LOCAL_CXXDEFINES += \
	$(patsubst %, -DUSED_%, $(shell echo $(TV_COMPONENTS) | tr [a-z] [A-Z]))

LOCAL_CXXFLAGS := \
	-L$(LOCAL_PATH)/libdvbsi++/lib -ldvbsi++ \
	$(LOCAL_CXXFLAGS) \


#
# Test apps
#

APP_LD_LIBRARY_PATH := export LD_LIBRARY_PATH=$(OUT_PATH)/lib:$(LOCAL_PATH)/libdvbsi++/lib

APP_CXXINCLUDES := $(LOCAL_CXXINCLUDES)
APP_CXXDEFINES := $(LOCAL_CXXDEFINES)
APP_CXXFLAGS := \
	-L$(OUT_PATH)/lib -l$(LOCAL_MODULE) \
	-L$(LOCAL_PATH)/libdvbsi++/lib \
	$(LOCAL_CXXFLAGS) \

LOCAL_APPS := \
	$(LOCAL_PATH)/test/psip_test \

#
# PHONY
#
.PHONY: all
all: mkdir_object $(BUILD_OPTION) install

.PHONY: test
test: mkdir_object apps

.PHONY: clean
clean: clear rmdir_object rmdir_out

include $(LOCAL_PATH)/rule.mk
