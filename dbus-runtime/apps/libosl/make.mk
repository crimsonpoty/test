include $(BUILD_ROOT)/tools/library.mk

INCLUDE_DIR = \
			include	\
			$(APPS_DIR)/include	\
			$(LINUX)/usr/include	\
			$(LINUX)/usr/include/mtdutils	\
			$(APPLIBS_TARGET_INC_DIR)

#CFLAGS :=      $(filter-out -std=c89, $(CFLAGS))
#CFLAGS :=      $(filter-out -Wstrict-prototypes, $(CFLAGS))

CFLAGS += $(addprefix -I, $(INCLUDE_DIR))
CFLAGS += -DREENTRANT $(addprefix -I, $(INCLUDE_DIR))
CFLAGS += -pthread
LDFLAGS += -L$(BUILD_ROOT)/bin -lpthread

CFLAGS += -I$(APPLIBS_TARGET_INC_DIR)
LDFLAGS += -L$(APPLIBS_TARGET_LIB_DIR) -luriparser

ifeq ($(MTD_SUPPORT),y)
vpath %.c %.h  mtd
include  mtd/make.mk
CFLAGS += -I$(ROOTFS)/user/mtdutils/include
CFLAGS += -DMTD_SUPPORT=1
endif

OBJS += \
	hashmap.o	\
	osl_mutex.o	\
	osl_spin.o	\
	osl_semaphore.o	\
	osl_thread.o	\
	linkedlist.o	\
	stringtokenizer.o	\
	workerthread.o	\
	lazyworker.o	\
	osl_event.o	\
	osl_random.o	\
	ioutil.o	\
	osl_time.o	\
	tftp.o	\
	properties.o	\
	osl_file.o	\
	osl_system.o	\
	osl_tty.o   \
	osl_net.o	\
	base64.o	\
	osl_eventque.o
	



