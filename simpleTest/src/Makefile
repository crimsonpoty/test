BUILD_ROOT = $(shell cd ../;pwd)

OBJ_DIR = $(BUILD_ROOT)/obj
BIN_DIR = $(BUILD_ROOT)/bin

TARGET = $(sort $(subst .cpp, , $(shell ls *.cpp)))

### Flag 선언
CXXFLAGS = -Wall -W -g
CXXFLAGS += -I$(BUILD_ROOT)/include
CXXFLAGS += -std=c++11
CXXFLAGS += $(filter-out -Wunused-parameter, $(CXXFLASG))

.PHONY: all

all: $(TARGET)

$(TARGET): %: %.cpp
	@echo "[Build... $(notdir $@)]"
	@$(CC) -o $(BIN_DIR)/$@ $? $(CXXFLAGS)

	
### Include dependencies
DEP_FILES := $(wildcard $(OBJ_DIR)/*.d)
ifneq ($(DEP_FILES),)
-include $(DEP_FILES)
endif
