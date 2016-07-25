# ������Ʈ �� ����
export PROJECT_NAME=WebUI
export PROJECT_PLATFORM=97439

# CrossTool ����
export BF_COMPILE_HOST=arm-linux
export BF_CROSS_COMPILE=$(TOOLCHAIN_DIR)/bin/$(BF_COMPILE_HOST)-
export TOOLCHAIN_DIR=/opt/crosstools/stbgcc-4.8-1.4
BUILD_PATH=$$PATH:$(TOOLCHAIN_DIR)/bin

# Library Ÿ�� ���� (y=Dynamic Library, n=Static Library)
export PROJECT_LIB_SHAREABLE=y

# ����� ��� ����
RESULT_DEBUG=y

# Build ����� ��� ����
export BUILT_BIN_DIR=$(BUILD_ROOT)/usr/local/bin
export BUILT_LIB_DIR=$(BUILT_BIN_DIR)/../lib
export BUILT_HEADER_DIR=$(BUILT_BIN_DIR)/../include
export MY_WORK_DIR=/opt/nfs/$(shell echo `whoami`)/$(PROJECT_PLATFORM)_$(PROJECT_NAME)
