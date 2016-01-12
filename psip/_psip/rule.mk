#!bin/sh
# rule.mk
#
# 2015.04.01
# erato.
#

#
# mkdir
#
mkdir_out:
	$(AT) if [ ! -d $(OUT_PATH) ]; then mkdir -p $(OUT_PATH); fi

mkdir_module:
	$(AT) if [ ! -d $(MODULE_PATH) ]; then mkdir -p $(MODULE_PATH); fi

mkdir_object:
	$(AT) if [ ! -d $(OBJECT_PATH) ]; then mkdir -p $(OBJECT_PATH); fi

#
# rmdir
#
rmdir_out:
	$(AT) if [ -d $(OUT_PATH) ]; then rm -rf $(OUT_PATH); fi

rmdir_object:
	$(AT) if [ -d $(OBJECT_PATH) ]; then rm -rf $(OBJECT_PATH); fi

build_log:
	$(AT) echo "[Build... $(LOCAL_MODULE)]"

clear:
	$(AT) echo "[Clean... $(LOCAL_MODULE)]"

vpath %.c $(LOCAL_SRC_PATH)
vpath %.cpp $(LOCAL_SRC_PATH)

#
# compile object. 
#
$(OBJECT_PATH)/%.o: %.c
	$(AT) echo [Compile... $(CC) $*]
	$(AT) $(CC) -fPIC $(LOCAL_CDEFINES) $(LOCAL_CINCLUDES) $(LOCAL_CFLAGS) -c -o $@ $<

$(OBJECT_PATH)/%.o: %.cpp
	$(AT) echo [Compile... $(CXX) $*]
	$(AT) $(CXX) -fPIC $(LOCAL_CXXDEFINES) $(LOCAL_CXXINCLUDES) $(LOCAL_CXXFLAGS) -c -o $@ $<

#
# implicit rule for building local apps 
#
APP_LD_ENV_NAME := app_ld_library_path.env
app_ld_lib_path:
	$(AT) if [ ! -d $(TARGET_PATH)/bin ]; then mkdir -p $(TARGET_PATH)/bin; fi 
	$(AT) if [ ! -f $(TARGET_PATH)/bin/$(APP_LD_ENV_NAME) ]; then \
		touch $(TARGET_PATH)/bin/$(APP_LD_ENV_NAME); \
		echo '$(APP_LD_LIBRARY_PATH)' > $(TARGET_PATH)/bin/$(APP_LD_ENV_NAME); \
	fi

%$(APPSUFFIX): %.c $(patsubst %.o,%.c,$(EXTRA_OBJS))
	$(AT) echo [Compile... $(CC) $*]
	$(AT) $(CC) -o $@ $(filter %.c %.s %.o, $^) $(APP_CDEFINES) $(APP_CINCLUDES) $(APP_CFLAGS)
	$(AT) mv -f $@ $(TARGET_PATH)/bin

%$(APPSUFFIX): %.cpp $(patsubst %.o,%.cpp,$(EXTRA_OBJS))
	$(AT) echo [Compile... $(CXX) $*]
	$(AT) echo "APP_CXXFLAGS = " $(APP_CXXFLAGS)
	$(AT) $(CXX) -o $@ $(filter %.cpp %.s %.o, $^) $(APP_CXXDEFINES) $(APP_CXXINCLUDES) $(APP_CXXFLAGS)
	$(AT) mv -f $@ $(TARGET_PATH)/bin

#	
# object files..
#
LOCAL_OBJECT_FILES := $(patsubst %.c, $(OBJECT_PATH)/%.o, $(notdir $(LOCAL_SRC_FILES)))
LOCAL_OBJECT_FILES := $(patsubst %.cpp, $(OBJECT_PATH)/%.o, $(notdir $(LOCAL_SRC_FILES)))

#
# build
#
ifeq ($(BUILD_OPTION), shared_lib)
TARGET_MODULE := lib$(LOCAL_MODULE).so
else
TARGET_MODULE := lib$(LOCAL_MODULE).a
endif

shared_lib: build_log $(LOCAL_OBJECT_FILES)
	$(AT) $(CXX) -shared -o $(LOCAL_PATH)/$(TARGET_MODULE) $(LOCAL_OBJECT_FILES)

static_lib: build_log $(LOCAL_OBJECT_FILES)
	$(AT) ar rcvs $(LOCAL_PATH)/$(TARGET_MODULE) -o $(LOCAL_OBJECT_FILES)

exe: build_log
	 $(AT) $(CXX) -o $(LOCAL_PATH)/$(LOCAL_MODULE) $(LOCAL_SRC_FILES) $(LOCAL_CINCLUDES) $(LOCAL_CFLAGS)

apps: build_log $(LOCAL_APPS) app_ld_lib_path

install:
	$(AT) echo "[Install... $(LOCAL_MODULE)]"
	$(AT) if [ ! -d $(TARGET_PATH) ]; then mkdir -p $(TARGET_PATH); fi 
	$(AT) if [ ! -d $(TARGET_PATH)/include ]; then mkdir -p $(TARGET_PATH)/include; fi 
	$(AT) if [ ! -d $(TARGET_PATH)/lib ]; then mkdir -p $(TARGET_PATH)/lib; fi 
	$(AT) if [ ! -d $(TARGET_PATH)/bin ]; then mkdir -p $(TARGET_PATH)/bin; fi 
	$(AT) for dir in $(LOCAL_INC_PATH); do \
		cp -rf $$dir/*.h $(TARGET_PATH)/include 2>/dev/null;true; \
	done 
	$(AT) if [ -f $(TARGET_MODULE) ]; then mv $(TARGET_MODULE) $(TARGET_PATH)/lib; fi
	$(AT) if [ -f $(LOCAL_MODULE) ]; then mv $(LOCAL_MODULE) $(TARGET_PATH)/bin; fi

