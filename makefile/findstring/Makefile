# BOARD = NonTelefonica				# correct
# BOARD = Telefonica				# correct
# BOARD = TelefonicaIsdbT			# correct
BOARD = TelefonicaChileIsdbT		# correct
# BOARD = TelefoncaChileIsdbT		# worng

ifeq ($(findstring Telefonica, $(BOARD)), Telefonica)
STRING = "BOARD=$(BOARD)"
endif

ifeq ($(STRING),)
STRING = "STRING is null!!"
endif

.PHONY: all

all:
	@echo $(STRING)
