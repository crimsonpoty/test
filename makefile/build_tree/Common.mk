MAKE = make
CXX = $(BF_CROSS_COMPILE)g++
CC  = $(BF_CROSS_COMPILE)gcc
AS  = $(BF_CROSS_COMPILE)as
LD  = $(BF_CROSS_COMPILE)g++
AR  = $(BF_CROSS_COMPILE)ar
NM  = $(BF_CROSS_COMPILE)nm
STRIP = $(BF_CROSS_COMPILE)strip
OBJCOPY = $(BF_CROSS_COMPILE)objcopy
OBJDUMP = $(BF_CROSS_COMPILE)objdump
RANLIB = $(BF_CROSS_COMPILE)ranlib

# These are operations common to all environments.
MKDIR   = mkdir -p
PWD 	= pwd
MV  	= mv
CP  	= cp -f
CD  	= cd
RM  	= rm -f
SORT	= sort
SED 	= sed
TOUCH   = touch

# make -j ¿É¼Ç¿¡ µû¸¥ CPU ¼ö
CPU_NUM = $(shell echo `cat /proc/cpuinfo | grep cores | wc -l`)

# ANSI ÄÃ·¯ ¼³Á¤
RED = "[31m"
GREEN = "[32m"
BROWN = "[33m"
BLUE = "[34m"
VIOLET = "[35m"
CYAN = "[36m"
GRAT = "[37m"
END_COLOR  = "[0m"
