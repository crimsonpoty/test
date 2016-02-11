CURRENT_DIR = $(shell echo pwd)
SRC_DIR = $(CURRENT_DIR)
TARGET_DIR = $(CURRENT_DIR)
CPU_NUM = $(shell echo `cat /proc/cpuinfo | grep cores | wc -l`)
MORECPUNUM = 1
BESTNUM = $(shell echo `expr "$(CPU_NUM)""+""$(MORECPUNUM)"`)
TARGET = $(subst .cpp, , $(shell ls *.cpp))

CC = g++
CROSS_COMPILE = /opt/crosstools/stbgcc-4.5.3-2.4/bin/mipsel-linux-uclibc-g++
#CC = $(CROSS_COMPILE)

.PHONY: all clean $(TARGET) test

all: $(TARGET)

$(TARGET): %: %.cpp
	@echo "[Complie $(notdir $@)]"
	@$(CC) -Wall -W -g -o $@ $<

clean:
	-@rm $(TARGET)

test: 
	@echo "[CPU_NUM is $(CPU_NUM)]"
	@echo "[MORECPUNUM is $(MORECPUNUM)]"
	@echo "[BESTNUM is $(BESTNUM)]"
	