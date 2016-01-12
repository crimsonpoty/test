CURRENT_DIR = $(shell echo `pwd`)
SRC_DIR = $(CURRENT_DIR)
TARGET_DIR = $(CURRENT_DIR)
CPUNUM = $(shell echo `cat /proc/cpuinfo | grep cores | wc -l`)
MORECPUNUM = 1
BESTNUM = $(shell echo `expr "$(CPUNUM)""+""$(MORECPUNUM)"`)
TARGET = test_time

CC = g++
CROSS_COMPILE = /opt/crosstools/stbgcc-4.5.3-2.4/bin/mipsel-linux-uclibc-g++
#CC = $(CROSS_COMPILE)

.PHONY: $(TARGET) clean

all:	
	@echo "[Complie $(TARGET)]"
#	@echo "[CPUNUM is $(CPUNUM)]"
#	@echo "[MORECPUNUM is $(MORECPUNUM)]"
#	@echo "[BESTNUM is $(BESTNUM)]"
	@$(CC) -Wall -W -g -o $(TARGET_DIR)/$(TARGET) $(SRC_DIR)/$(TARGET).cpp

clean:
	rm -f $(CURRENT_DIR)/$(TARGET)
