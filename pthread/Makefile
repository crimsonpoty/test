CURRENT_DIR = $(shell echo `pwd`)
SRC_DIR = $(CURRENT_DIR)
TARGET_DIR = $(CURRENT_DIR)
CPUNUM = $(shell echo `cat /proc/cpuinfo | grep cores | wc -l`)
MORECPUNUM = 1
BESTNUM = $(shell echo `expr "$(CPUNUM)""+""$(MORECPUNUM)"`)
TARGET = pthread_example

C_FLAGS += -lpthread

CROSS_COMPILE = /opt/crosstools/stbgcc-4.5.3-2.4/bin/mipsel-linux-uclibc-g++
CC = g++
# CC = $(CROSS_COMPILE)

.PHONY: all clean $(TARGET)

all: $(TARGET)

$(TARGET):
	@echo "[Complie $@]"
	@$(CC) -Wall -W -g -o $@ $@.cpp $(C_FLAGS)

clean:
	-rm -f $(CURRENT_DIR)/$(TARGET)
