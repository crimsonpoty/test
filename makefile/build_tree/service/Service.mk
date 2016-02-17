#### Service Config ÆÄÀÏ

## Benjamin libraries LDFLAGS
APP_MANAGER_LDFLAGS = -lpthread -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lz -lffi -Wl,--export-dynamic -lgmodule-2.0 -lintl -ljson++
MEDIA_LDFLAGS		= -lpthread -lrt -lm -ldl  -lstdc++ -lnexus -lnexus_client -lnxclient -lb_os -lb_playback_ip -lssl -lcrypto -lliveMedia -lgroupsock -lUsageEnvironment -lBasicUsageEnvironment -lblive_ext -ldvbsi++ -lxml2
OSD_LDFLAGS			= -lpthread -lrt -lnexus_client -lnxclient
PLATFORM_LDFLAGS	= -lpthread -lrt -lm -ldl -lnexus
SETTINGS_LDFLAGS	=


## include files
ifeq ($(notdir $(CURRENT_DIR)), service)
endif


####################################################
##                   INCLUDE_DIR                  ##
####################################################
SERVICE_INCLUDE_DIRS += $(BUILT_HEADER_DIR)


####################################################
##                     DEFINES                    ##
####################################################
SERVICE_CDEFINES +=
SERVICE_CXXEFINES += $(SERVICE_CDEFINES)


####################################################
##                   C(XX)FLAGS                   ##
####################################################
SERVICE_CFLAGS += -pthread -W -Wall -g
SERVICE_CFLAGS += -std=c11
SERVICE_CXXFLAGS += $(filter-out -std=c11, $(SERVICE_CFLAGS))
SERVICE_CXXFLAGS += -std=c++11


####################################################
##                    LDFLAGS                     ##
####################################################
LDFLAGS += -L$(BUILT_LIB_DIR)

