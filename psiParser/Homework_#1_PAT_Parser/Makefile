TARGET = TS_Parser

SRCS =	Main.cpp \
		Program_Association_Section.cpp \
		Program.cpp \
		Program_Map_Section.cpp \
		Network_Information_Section.cpp \
		VirtualChannelTable.cpp \
		Common.cpp

CXXFLAGS = -Wall -W -g
LDFLAGS =


MAKE = make
CXX = g++
CC  = gcc
AS  = as
LD  = g++
AR  = ar
NM  = nm
STRIP = strip
OBJCOPY = objcopy
OBJDUMP = objdump
RANLIB = ranlib

MKDIR   = mkdir -p
PWD 	= pwd
MV  	= mv
CP  	= cp -f
CD  	= cd
RM  	= rm -f
SORT	= sort
SED 	= sed
TOUCH   = touch

# ANSI ÄÃ·¯ ¼³Á¤
RED = "[31m"
GREEN = "[32m"
BLUE = "[34m"
VIOLET = "[35m"
END_COLOR  = "[0m"

CURRENT_DIR = $(shell echo `pwd`)
OBJ_DIR = $(CURRENT_DIR)/obj
BIN_DIR = $(CURRENT_DIR)/bin

VPATH = %.c %.cpp %.d %.h \
$(CURRENT_DIR)/obj

OBJS := $(patsubst %.cpp,%.o,$(notdir $(patsubst %.c,%.o,$(notdir $(SRCS)))))
OBJECTS = $(foreach OBJS,$(OBJS),$(OBJ_DIR)/$(OBJS))


.PHONY: $(TARGET) clean

$(TARGET): $(OBJS)
	@test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)
	@$(LD) -o $(BIN_DIR)/$(TARGET) $(OBJECTS) $(LINK_OPTION) -Xlinker --start-group $(LDFLAGS) -Xlinker --end-group

clean:
	$(RM) $(OBJ_DIR)/*.[od]
	$(RM) $(BIN_DIR)/$(TARGET)

.cpp.o:
	@test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)
	@$(CXX) -MT $@ $< -MM -MP -MF $(subst .cpp,.d, $(OBJ_DIR)/$(notdir $<)) $(CXXFLAGS)
	@echo "$(GREEN)[Compile $<]$(END_COLOR)"
	@$(CXX) -o $(OBJ_DIR)/$@ -c $< $(CXXFLAGS)
