CURRENT_DIR = $(shell pwd)

SRC_DIR = $(CURRENT_DIR)/src
OBJ_DIR = $(CURRENT_DIR)/obj
BIN_DIR = $(CURRENT_DIR)/bin

SRCS = $(sort $(notdir $(subst .cpp, , $(shell ls $(SRC_DIR)/*.cpp))))

CPU_NUM = $(shell echo `cat /proc/cpuinfo | grep cores | wc -l`)
MORE_CPU_NUM = 1
BEST_NUM = $(shell expr $(CPU_NUM) + $(MORE_CPU_NUM))



.PHONY: all clean

all: check_dir
	@make -j$(BEST_NUM) -C $(SRC_DIR)

clean:
	-@rm -rf $(OBJ_DIR)
	-@rm -rf $(BIN_DIR)

check_dir:
	@test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)
	@test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)

test:
	@echo $(BEST_NUM)