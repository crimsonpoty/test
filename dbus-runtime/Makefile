export BUILD_ROOT=$(shell echo `pwd`)

include $(BUILD_ROOT)/tools/config.mk
include $(BUILD_ROOT)/tools/tools.mk

#TARGETS = opensource apps
TARGETS = apps opensource

.PHONY : all $(TARGETS) veryclean install uninstall gdbus client
all :  apps

$(TARGETS) :
	$(call DISPLAY_RED , $(subst ,,* $@ ) )
	@ $(MAKE) -C $@

#clean : $(patsubst %, %_clean, $(TARGETS))
#	$(call DISPLAY_RED ,##  $@  COMPLETE !!!  )
clean : apps_clean
	$(call DISPLAY_RED ,##  $@  COMPLETE !!!  )


$(patsubst %,%_clean,$(TARGETS)) :
	$(Q_)$(MAKE) -C $(subst _clean,,$@) clean
	
install : $(HOME)/bin $(patsubst %, %_install, $(TARGETS))
	$(Q_)$(CP) $(HTML_ROOT) $(HOME)/bin
	$(call DISPLAY_RED ,##  $@  COMPLETE !!!  )

$(patsubst %,%_install,$(TARGETS)) :
	$(Q_)$(MAKE) -C $(subst _install,,$@) install

uninstall :
	$(Q_)$(RM) -rf $(OBJ_DEST)
	
veryclean : uninstall clean
	@ $(RM) -rf $(BUILD_ROOT)/output

gdbus : $(patsubst %, %_gdbus, $(TARGETS))
	$(call DISPLAY_RED ,##  $@  COMPLETE !!!  )

$(patsubst %,%_gdbus,$(TARGETS)) :
	$(Q_)$(MAKE) -C $(subst _gdbus,,$@) gdbus 
	
#client : $(patsubst %, %_client, $(TARGETS))
#	$(call DISPLAY_RED ,##  $@  COMPLETE !!!  )

#$(patsubst %,%_client,$(TARGETS)) :
#	$(Q_)$(MAKE) -C $(subst _client,,$@) client

client : $(patsubst %, %_client, $(TARGETS))
	$(call DISPLAY_RED ,##  $@  COMPLETE !!!  )

$(patsubst %,%_client,$(TARGETS)) :
	$(Q_)$(MAKE) -C $(subst _client,,$@) client 

$(HOME)/bin :
	$(Q_)$(MKDIR) $@
	
include $(BUILD_ROOT)/tools/print.mk
