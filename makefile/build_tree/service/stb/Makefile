CURRENT_DIR = $(shell pwd)
BUILD_ROOT  = $(shell cd ../../;pwd)

#
# 빌드할 모듈 이름 지정
#
STB_SERVICE_NAME = Stb

include Stb.mk
include $(BUILD_ROOT)/service/Makefile

clean:
	@$(RM) $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(notdir $(patsubst %.c,%.o,$(notdir $(STB_SRCS))))))
	@$(RM) $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.d,$(notdir $(patsubst %.c,%.d,$(notdir $(STB_SRCS))))))
	@$(RM) $(OBJ_DIR)/$(notdir $(SERVICE_FULL_NAMES))
	@$(RM) $(BUILT_BIN_DIR)/$(notdir $(SERVICE_FULL_NAMES))

test2:
	@echo $(notdir $(CURRENT_DIR))