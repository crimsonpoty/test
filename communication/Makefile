CURRENT_DIR = $(shell echo `pwd`)
SRC_DIR = $(CURRENT_DIR)
TARGET_DIR = $(CURRENT_DIR)
CPUNUM = $(shell echo `cat /proc/cpuinfo | grep cores | wc -l`)
MORECPUNUM = 1
BESTNUM = $(shell echo `expr "$(CPUNUM)""+""$(MORECPUNUM)"`)
TARGET_SERVER = print_server
TARGET_CLIENT = msg_client
TARGET_MSG_SENDER = msg_sender
TARGET_MSG_RECEIVERT = msg_receiver

C_FLAGS += -lpthread

CROSS_COMPILE = /opt/crosstools/stbgcc-4.5.3-2.4/bin/mipsel-linux-uclibc-g++
CC = g++
# CC = $(CROSS_COMPILE)

.PHONY: all clean $(TARGET_SERVER) $(TARGET_CLIENT)

all: $(TARGET_SERVER) $(TARGET_CLIENT) $(TARGET_MSG_SENDER) $(TARGET_MSG_RECEIVERT)

$(TARGET_SERVER):
	@echo "[Complie $@]"
	@$(CC) -Wall -W -g -o $@ $@.cpp

$(TARGET_CLIENT):
	@echo "[Complie $@]"
	@$(CC) -Wall -W -g -o $@ $@.cpp $(C_FLAGS)

$(TARGET_MSG_SENDER):
	@echo "[Complie $@]"
	@$(CC) -Wall -W -g -o $@ $@.cpp

$(TARGET_MSG_RECEIVERT):
	@echo "[Complie $@]"
	@$(CC) -Wall -W -g -o $@ $@.cpp

clean:
	-rm -f $(CURRENT_DIR)/$(TARGET_SERVER)
	-rm -f $(CURRENT_DIR)/$(TARGET_CLIENT)
	-rm -f $(CURRENT_DIR)/$(TARGET_MSG_SENDER)
	-rm -f $(CURRENT_DIR)/$(TARGET_MSG_RECEIVERT)

