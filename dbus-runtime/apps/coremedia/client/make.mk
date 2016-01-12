include $(BUILD_ROOT)/tools/config.mk
include $(BUILD_ROOT)/tools/library.mk

INCLUDE_DIR = \
	.	\
	./include	\
	../include	\
	../gdbus	\
	${APPS_DIR}/libosl/include	\

CFLAGS += $(addprefix -I, $(INCLUDE_DIR))

CFLAGS += `pkg-config --cflags glib-2.0`
CFLAGS += `pkg-config --cflags gio-unix-2.0`

LDFLAGS += `pkg-config --libs glib-2.0`
LDFLAGS += `pkg-config --libs gio-unix-2.0`

#LDFLAGS += -lffi -lz

vpath %.c ../gdbus
include ../gdbus/make.mk

OBJS += \
	client.o	\
	DataSink.o	\
	DataSource.o	\
	Source.o	\
	Sink.o	\
	SourcePool.o	\
	SinkPool.o	\
	DBusObjectManager.o	\
#	HttpSource.o	\

	#	MediaPlayer.o	\
	