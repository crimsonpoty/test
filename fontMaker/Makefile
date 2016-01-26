# Make 시 verbose 모드를 결정한다. ('V=1' 아규먼트가 있으면 verbose 모드, 없으면 silent 모드)
ifdef V
QM =
else
QM = @
endif

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


CURRENT_DIR = $(shell echo `pwd`)
FREETYPE_DIR = $(CURRENT_DIR)/freetype-2.5.5
BOOST_DIR = /opt/crosstools/boost_1_49_0
ROOT_DIR = $(CURRENT_DIR)/../../../..

TARGET = FontMaker


OBJ_DIR = $(CURRENT_DIR)/obj
BIN_DIR = $(CURRENT_DIR)/bin

VPATH = %.c %.cpp %.d %.h \
$(CURRENT_DIR)/obj

SRCS = main.cpp

OBJS := $(patsubst %.cpp,%.o,$(notdir $(patsubst %.c,%.o,$(notdir $(SRCS)))))
OBJECTS = $(foreach OBJS,$(OBJS),$(OBJ_DIR)/$(OBJS))

CXXFLAGS = -I$(FREETYPE_DIR)/include -I$(BOOST_DIR) -I$(ROOT_DIR)/utils/inc
LDFLAGS = $(OBJ_DIR)/libfreetype.a

.PHONY: $(TARGET) clean freetype

$(TARGET): $(OBJS)
	@test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)
	@$(LD) -o $(BIN_DIR)/$(TARGET) $(OBJECTS) $(LINK_OPTION) -Xlinker --start-group $(LDFLAGS) -Xlinker --end-group 

clean:
	$(RM) $(OBJ_DIR)/*.[od]
	$(RM) $(BIN_DIR)/$(TARGET)

freetype: freetype_configure
	@$(MAKE) -C $(FREETYPE_DIR)
	@$(CP) $(FREETYPE_DIR)/objs/.libs/libfreetype.a $(OBJ_DIR)/

freetype_configure:
ifeq (exist, $(shell [ -e $(FREETYPE_DIR)/config.mk ] && echo exist))
	@echo "$(RED)freetype already configured!!$(END_COLOR)"
else
	@cd $(FREETYPE_DIR);export LD=$(LD);$(FREETYPE_DIR)/configure
endif

freetype_clean:
	@echo "$(RED)$@$(END_COLOR)"
ifeq (exist, $(shell [ -e $(FREETYPE_DIR)/config.mk ] && echo exist))
	@$(MAKE) distclean -C $(FREETYPE_DIR)
endif
	$(RM) $(OBJ_DIR)/libfreetype.a

.cpp.o:
	@test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)
	@$(CXX) -MT $@ $< -MM -MP -MF $(subst .cpp,.d, $(OBJ_DIR)/$(notdir $<)) $(CXXFLAGS)
	@echo "$(GREEN)[Compile $<]$(END_COLOR)"
	@$(CXX) -o $(OBJ_DIR)/$@ -c $< $(CXXFLAGS)

.c.o:
	@test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)
	@$(CC) -MT $@ $< -MM -MP -MF $(subst .c,.d, $(OBJ_DIR)/$(notdir $<) ) $(CXXFLAGS)
	@echo "$(BLUE)[Compile $<]$(END_COLOR)"
	$(QM)$(CC) -o $(OBJ_DIR)/$@ -c $< $(CXXFLAGS)

-include $(OBJ_DIR)/*.d
