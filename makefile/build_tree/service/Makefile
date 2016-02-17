ifeq ($(notdir $(shell pwd)), service)
CURRENT_DIR = $(shell pwd)
BUILD_ROOT  = $(shell cd ../;pwd)

#
# 빌드할 모듈 이름 지정(안하는 모듈은 주석처리)
#
STB_SERVICE_NAME	= Stb
endif


include $(BUILD_ROOT)/Config.mk
include $(BUILD_ROOT)/service/Service.mk
-include $(CURRENT_DIR)/stb/Stb.mk

include $(BUILD_ROOT)/Common.mk

SERVICE_NAMES := $(STB_SERVICE_NAME)
SERVICE_FULL_NAMES := $(addsuffix Service, $(SERVICE_NAMES))


OBJ_DIR := $(BUILD_ROOT)/service/obj

STB_OBJS := $(addprefix $(OBJ_DIR)/,$(sort $(patsubst %.cpp,%.o,$(notdir $(patsubst %.c,%.o,$(notdir $(STB_SRCS)))))))

#
# 빌드 Flag 설정
#
UPPER_SERVICE_COMPONENTS := $(shell echo $(shell echo `ls $(BUILD_ROOT)/service`) | tr [a-z] [A-Z])
INCLUDE_DIRS:= $(addprefix -I, $(foreach services, SERVICE $(UPPER_SERVICE_COMPONENTS), $($(services)_INCLUDE_DIRS)))
CDEFINES	:= $(addprefix -D, $(foreach services, SERVICE $(UPPER_SERVICE_COMPONENTS), $($(services)_CDEFINES)))
CXXDEFINES	:= $(addprefix -D, $(foreach services, SERVICE $(UPPER_SERVICE_COMPONENTS), $($(services)_CXXDEFINES)))
CFLAGS		:= $(INCLUDE_DIRS) $(CDEFINES) $(foreach services, SERVICE $(UPPER_SERVICE_COMPONENTS), $($(services)_CFLAGS))
CXXFLAGS	:= $(INCLUDE_DIRS) $(CXXDEFINES) $(foreach services, SERVICE $(UPPER_SERVICE_COMPONENTS), $($(services)_CXXFLAGS))


#
# 빌드 타겟 설정
#
TARGETS := $(addprefix $(OBJ_DIR)/, $(SERVICE_FULL_NAMES))

.PHONY:  all clean check_dir

all: check_dir $(TARGETS)

test:
	@echo $(notdir $(CURRENT_DIR))

# Build binary of STB_SERVICE
ifneq ($(STB_SERVICE_NAME),)
$(OBJ_DIR)/$(STB_SERVICE_NAME)Service: $(notdir $(STB_OBJS))
	@echo "$(CYAN)[Build... $(notdir $@)]$(END_COLOR)"
	@$(LD) -o $@ $(STB_OBJS) -Xlinker --start-group $(LDFLAGS) $(STB_LDFLAGS) -Xlinker --end-group
	@echo "$(RED)[Copy... $(notdir $@) to $(BUILT_BIN_DIR)]$(END_COLOR)"
	@$(CP) $@ $(BUILT_BIN_DIR)
endif


#
# Object 경로 설정
#
vpath %.o $(OBJ_DIR)

#
# Flag to generate dependencies
#
DEP_FLAGS := -MM -MP -MF
 #-MM -MP -MF

#
# Compile cpp, cfiles
#
%.o: %.cpp
	@echo "$(GREEN)[Compile... $(notdir $<)]$(END_COLOR)"
	@$(CXX) -MT $@ $< -MM -MP -MF $(subst .cpp,.d, $(OBJ_DIR)/$(notdir $<)) $(CXXFLAGS)
	@$(CXX) -o $(OBJ_DIR)/$@ -c $< $(CXXFLAGS)

%.o: %.c
	@echo "$(BLUE)[Compile... $(notdir $<)]$(END_COLOR)"
	@$(CC) -MT $@ $< $(DEP_FLAGS) $(subst .c,.d, $(OBJ_DIR)/$(notdir $<)) $(CFLAGS)
	@$(CC) -o $(OBJ_DIR)/$@ -c $< $(CFLAGS)

check_dir:
	@test -d $(OBJ_DIR) || $(MKDIR) $(OBJ_DIR)
	@test -d $(BUILT_LIB_DIR) || echo "Benjamin libraries do not exist!!!"

ifeq ($(notdir $(shell pwd)), service)
clean:
	@echo "$(RED)[Clean... Service Objects]$(END_COLOR)"
	@if [ -d $(OBJ_DIR) ]; then \
		$(RM) -r $(OBJ_DIR); \
	fi
	@echo "$(RED)[Clean... Service Binaries]$(END_COLOR)"
	@$(RM) $(addprefix $(BUILT_BIN_DIR)/, $(notdir $(SERVICE_FULL_NAMES)))
endif

#
# Include dependencies
#
SERVICE_DEP_FILES := $(wildcard $(OBJ_DIR)/*.d)
ifneq ($(SERVICE_DEP_FILES),)
-include $(SERVICE_DEP_FILES)
endif
