include $(BUILD_ROOT)/tools/tools.mk
include $(BUILD_ROOT)/tools/print.mk
include $(BUILD_ROOT)/tools/library.mk

INCLUDE_DIR =	\
		include	\
		flash	\
		$(LINUX)	\
		$(LINUX)/usr/include	\
		$(LINUX)/usr/include/mtdutils	\
			

CFLAGS += $(addprefix -I, $(INCLUDE_DIR))


OBJS += \
	nandflash_priv.o	\
	norflash_priv.o	\
	osl_mtd.o

