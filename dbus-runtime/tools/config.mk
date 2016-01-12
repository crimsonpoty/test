#select config files
BUILD_ROOT ?= ..

export BIN_DIR = $(BUILD_ROOT)/bin
export LIB_DIR = $(BIN_DIR)
APPS_DIR = $(BUILD_ROOT)/apps

OPENSOURCE_DIR = $(BUILD_ROOT)/opensource
OPENSOURCE_PREFIX = $(OPENSOURCE_DIR)/$(ARCH)

export PROJECT_DOMAIN = com.halkamalka
export PROJECT_CODE = BF
export GDBUS_CODEGEN_DIR = /usr/bin

export PKG_CONFIG_PATH = $(OPENSOURCE_PREFIX)/usr/local/lib/pkgconfig

#ifeq ($(DEBUG),n)
#OBJ_DIR:= $(BUILD_ROOT)/release
#else
#OBJ_DIR:= $(BUILD_ROOT)/debug
#endif

TARGET_TOP = $(OPENSOURCE_DIR)/../target
WPA_SUPPLICANT_SRC=$(OPENSOURCE_DIR)/wpa_supplicant/wpa_supplicant-2.0/src

HTML_ROOT = $(BUILD_ROOT)/webapp

