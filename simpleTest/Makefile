CURRENT_DIR = $(shell pwd)

SRC_DIR = $(CURRENT_DIR)/src
OBJ_DIR = $(CURRENT_DIR)/obj
BIN_DIR = $(CURRENT_DIR)/bin

CPU_NUM = $(shell echo `cat /proc/cpuinfo | grep cores | wc -l`)
MORE_CPU_NUM = 1
BEST_NUM = $(shell expr $(CPU_NUM) + $(MORE_CPU_NUM))

BUILD_COMPILER = g++
CROSS_COMPILE = /opt/crosstools/stbgcc-4.5.3-2.4/bin/mipsel-linux-uclibc-g++
#BUILD_COMPILER = $(CROSS_COMPILE)

.PHONY: all clean

all: check_dir
	@export CC=$(BUILD_COMPILER);make -j$(BEST_NUM) -C $(SRC_DIR)

clean:
	-@rm -rf $(OBJ_DIR)
	-@rm -rf $(BIN_DIR)

check_dir: 
	@test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)
	@test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)
	
test:
	@echo $(BEST_NUM)