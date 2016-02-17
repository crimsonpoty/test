BUILD_ROOT = $(shell pwd)

include Common.mk

BUILD_COMPONENTS = service
BUILD_COMPONENTS_CLEAN = service

.PHONY: all clean

all: 
	@for i in $(BUILD_COMPONENTS); do \
		echo "$(RED)[Building... $$i]$(END_COLOR)"; \
		$(MAKE) -j$(CPU_NUM) -C $$i || exit 1; \
		echo "$(RED)[Building... $$i Finished]$(END_COLOR)"; \
	done

clean:
	@for i in $(BUILD_COMPONENTS_CLEAN); do \
		echo "$(RED)[Cleaning... $$i]$(END_COLOR)";\
		$(MAKE) -j$(CPU_NUM) -C $$i clean --silent || exit 1; \
		echo "$(RED)[Cleaning... $$i Finished]$(END_COLOR)"; \
	done