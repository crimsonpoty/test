include $(BUILD_ROOT)/tools/library.mk

INCLUDE_DIR += \
			include	\
			$(APPS_DIR)/libjson++/include	\
			$(APPS_DIR)/libjson++	\
			$(APPS_DIR)/libut++/include	\
			$(APPS_DIR)/libosl/include	\
			$(APP_DIR)/gdbus
			
CFLAGS += $(addprefix -I, $(INCLUDE_DIR))

CFLAGS += `pkg-config --cflags glib-2.0`
CFLAGS += `pkg-config --cflags gio-unix-2.0`

LDFLAGS += `pkg-config --libs glib-2.0`
LDFLAGS += `pkg-config --libs gio-unix-2.0`

vpath %.c %.h gdbus
include gdbus/make.mk

OBJS += \
	server.o	\
	SourcePoolImpl.o	\
	SinkPoolImpl.o	\
	DBusObjectManagerServer.o	\
	SourceImpl.o	\
	SinkImpl.o	\
#	DataSource.o	\
	DataSink.o	\
	MediaPlayer.o	\
