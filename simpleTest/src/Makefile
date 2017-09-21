BUILD_ROOT = $(shell cd ../;pwd)

OBJ_DIR = $(BUILD_ROOT)/obj
BIN_DIR = $(BUILD_ROOT)/bin

### 윈도우(MinGW) 환경으로 변경됨에 따라 리눅스에 의존성(socket, pthread, etc...)있는 파일은 제외
ifneq ($(findstring Linux, $(shell uname)), Linux)
	LINUX_DEP_FILES = test_aton test_stackoverflow
endif
TARGET = $(filter-out $(LINUX_DEP_FILES), $(sort $(subst .cpp, , $(shell ls *.cpp))))


### Flag 선언
CXXFLAGS = -Wall -W -g
CXXFLAGS += -I$(BUILD_ROOT)/include
CXXFLAGS += -std=c++11
CXXFLAGS += $(filter-out -Wunused-parameter, $(CXXFLASG))

### compiler 설정
CC = g++
CROSS_COMPILE = /opt/crosstools/stbgcc-4.5.3-2.4/bin/mipsel-linux-uclibc-g++
#CC = $(CROSS_COMPILE)


.PHONY: all $(TARGET)

all: $(TARGET)

$(TARGET): %: %.cpp
	@echo "[Build... $(notdir $@)]"
	@$(CC) -o $(BIN_DIR)/$@ $? $(CXXFLAGS)
	
	
### Include dependencies
DEP_FILES := $(wildcard $(OBJ_DIR)/*.d)
ifneq ($(DEP_FILES),)
-include $(DEP_FILES)
endif
