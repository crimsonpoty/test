include $(BUILD_ROOT)/tools/library.mk

INCLUDE_DIR = \
			include	\
			$(APPS_DIR)/libosl/include	\
			$(APPS_DIR)/libut++/include	\
			$(APPS_DIR)/libjson++/include	\
			
#			$(APP_DIR)/json++/include	\
			$(APP_DIR)/jsonrpc++/include

#CFLAGS :=      $(filter-out -std=c89, $(CFLAGS))
#CFLAGS :=      $(filter-out -Wstrict-prototypes, $(CFLAGS))

CFLAGS += $(addprefix -I, $(INCLUDE_DIR))
#CFLAGS += -pthread -DUSE_WEBSOCKET -pthread -g -pipe
LDFLAGS += -L$(BUILD_ROOT)/bin -lpthread -ldl
# -ljson++

vpath %.cpp %.h  jsonrpc++
include  jsonrpc++/make.mk

#vpath %.cpp %.h  json++
#include  json++/make.mk

OBJS += \
	mongoose.o	\
	webservice.o	\
	websocket.o	\
	json_websocket.o	\
	mgsocket.o	\
	httpservice_handler.o	\

