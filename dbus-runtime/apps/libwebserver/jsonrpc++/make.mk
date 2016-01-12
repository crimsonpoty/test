include $(BUILD_ROOT)/tools/library.mk

INCLUDE_DIR += \
			include	\
			$(APP_DIR)/json++/include	\
			$(APP_DIR)/jsonrpc++/include	\
			$(APP_DIR)/jsonrpc++/include/jsonrpc

CFLAGS += $(addprefix -I, $(INCLUDE_DIR))

OBJS += \
	system.o	\
	netstring.o	\
	jsonrpc_handler.o	\
	jsonrpc_handler_ex.o
	

