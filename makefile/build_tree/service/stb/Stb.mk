## dir ����
ifeq ($(notdir $(CURRENT_DIR)), service)
STB_DIR := $(CURRENT_DIR)/stb
else
STB_DIR := $(CURRENT_DIR)
endif

## include files


####################################################
##                   INCLUDE_DIR                  ##
####################################################
STB_INCLUDE_DIRS += $(STB_DIR)/include


####################################################
##                     DEFINES                    ##
####################################################
STB_CDEFINES +=
STB_CXXDEFINES += $(STB_CDEFINES)


####################################################
##                   C(XX)FLAGS                   ##
####################################################
STB_CFLAGS +=
STB_CXXFLAGS += $(STB_CFLAGS)


####################################################
##                    LDFLAGS                     ##
####################################################
STB_LDFLAGS +=


## �ҽ� ��ġ ����
vpath %.cpp $(STB_DIR)/src
vpath %.c $(STB_DIR)/src


STB_SRCS += \
	Osd.cpp	\
	main.cpp	\
