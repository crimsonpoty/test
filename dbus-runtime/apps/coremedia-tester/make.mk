include $(BUILD_ROOT)/tools/library.mk

INCLUDE_DIR = \
			include	\
			$(APPS_DIR)/libosl/include	\
			$(APPS_DIR)/libut++/include	\
			$(APPS_DIR)/libwebserver/include	\
			$(APPS_DIR)/libwebserver	\
			$(APPS_DIR)/libcoremedia	\
			$(APPS_DIR)/coremedia/client/include	\
			$(APPS_DIR)/libjson++/include	\
			$(APPS_DIR)/libwebserver/jsonrpc++/include	\
			
			

CFLAGS += $(addprefix -I, $(INCLUDE_DIR))
CFLAGS += -pthread -DUSE_WEBSOCKET -pthread -g -pipe
LDFLAGS += -L$(BUILD_ROOT)/bin -lpthread -ldl -lcoremedia -lwebserver -losl -lut++ -luriparser -ljson++ 


OBJS += \
	main.o	\
	HttpQuery.o	\
	Media.o	\
