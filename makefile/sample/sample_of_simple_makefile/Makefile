###############################
# Makefile ver 0.1 by greenfish
###############################
#
# make                          : debug 빌드
# make debug                    : debug 빌드 
# make release                  : release 빌드
# make clean [debug, release]   : 해당 빌드 경로 파일 삭제
# make rebuild [debug, release] : rebuild 실행

# 빌드의 기본값은 debug이며,
# 이곳 section에서는 debug와 release의 공통 부분을 정의한다.
# CFLAGS와 CPPFLAGS는 -c 옵션은 제외한다. rule에서 직접 -c를 넣어야 한다.
# LFLAGS는 Link 옵션으로 필요시 추가한다.
# config는 configuration으로 debug/release를 구별하며, 빌드 경로값에 영향을 준다.
# BUILD_DIR는 빌드에 사용될 경로를 지정한다.
# TARGET은 최종 빌드의 산출물을 지정한다. 빌드 상대 경로를 포함시킨다.
CC        = g++
CFLAGS    = -I/usr/local/include
CPPFLAGS  = -I/usr/local/include
LFLAGS    =
CONFIG    = debug
BUILD_DIR = ./Build/$(CONFIG)
TARGET    = $(BUILD_DIR)/Execute

##################################
# makefile에 들어온 argument를 구한다.
ifneq "$(findstring clean, $(MAKECMDGOALS))" ""
    ARG.CLEAN  = 1
endif

ifneq "$(findstring release, $(MAKECMDGOALS))" ""
    ARG.RELEASE = 1
endif

ifneq "$(findstring debug, $(MAKECMDGOALS))" ""
    ARG.DEBUG = 1
endif

ifneq "$(findstring rebuild, $(MAKECMDGOALS))" ""
    ARG.REBUILD = 1
endif

##################################
# DEBUG / RELEASE 빌드를 설정한다.
ifeq ($(ARG.RELEASE),1)
    # ------------------------------
    # release에 특화된 설정을 한다.
    # ------------------------------
    CFLAGS    += -O -DNDEBUG
    CPPFLAGS  += -O -DNDEBUG
    CONFIG    = release
else
    # ------------------------------
    # debug에 특화된 설정을 한다.
    # ------------------------------
    CFLAGS    += -DDEBUG -g
    CPPFLAGS  += -DDEBUG -g
    CONFIG    = debug
endif

##################################
# makefile 실행 처리

.PHONY: debug release build clean rebuild PRE_BUILD POST_BUILD all

# 일반적인 Build의 스텝을 결정한다.
BUILD_STEP = PRE_BUILD $(TARGET) POST_BUILD

# Makefile의 최종 Target과 Depend를 결정한다.
ifeq ($(ARG.REBUILD),1)
    # rebuild인 경우,...
    # 빌드 이전에 clean을 수행한다.
    rebuild: | clean $(BUILD_STEP)
    debug: ; @true
    release: ; @true
else ifeq ($(ARG.CLEAN),1)
    # clean인 경우,...
    # clean target은 rule part에 정의되어 있다.
    release: ; @true
    debug: ; @true
else
    # clean/rebuild와 함께 쓰이지 않은 경우,...
    # 이곳에서 빌드가 이뤄진다.
    # release, debug는 단독 사용할 수 있어 @true하지 않는다.
    build: | $(BUILD_STEP)
    release: build
    debug: build
endif

# release, debug가 명령에 포함되어 조합되면,
# release, debug target을 찾게 되는데,
# 의도하지 않은
#    "make: Nothing to be done for 'release'"
#    "make: Nothing to be done for 'debug'"
# 를 방지하기 위해 @true를 사용하였다.

# ------------------------------
# Other macro
# ------------------------------
# .o를 .d로 바꿔준다.
DEPEND_FILE = $(patsubst %.o,%.d,$@)

##################################
# 빌드 항목 구성
##################################

# Group은 빌드 환경 설정(컴파일 옵션, 빌드 경로, 소스 확장자, ...)이 같은 것을 묶은 것을 말한다.

#--------------------------------
# 주의 : ../../ 와 같이 상위 directory로 넘어가는 source는 가져오면 안된다.
# 왜냐하면, ./Build/ 밑으로 모든 빌드 파일이 들어가야 하는데,
# ../../등이 포함되면 ./Build/ 상위로 빌드 파일이 들어갈 수 있기 때문에, 위험하다.
# 즉, 다음의 기본 가정을 지켜야 한다.
# `makefile의 위치는 모든 빌드되는 source의 최상위 경로에 있어야 한다.'

##################################
# Group 01
GROUP.01.SRC = File1.cpp               \
               ./some_source/File3.cpp \
               ./some_source/File4.cpp
GROUP.01.OBJ = $(addprefix $(BUILD_DIR)/,$(GROUP.01.SRC:.cpp=.o))
GROUP.01.DEP = $(GROUP.01.OBJ:.o=.d)

##################################
# Group 02
GROUP.02.SRC = File2.c
GROUP.02.OBJ = $(addprefix $(BUILD_DIR)/,$(GROUP.02.SRC:.c=.o))
GROUP.02.DEP = $(GROUP.02.OBJ:.o=.d)

##################################
# [Link] Link Part
# 빌드된 .o 들을 링크하여 TARGET을 생성한다. 
$(TARGET): $(GROUP.01.OBJ) $(GROUP.02.OBJ)
	@echo ----------------------------------------
	@echo Link : $(TARGET)
	@echo ----------------------------------------
	$(CC) $(LFLAGS) $(GROUP.01.OBJ) $(GROUP.02.OBJ) -o $(TARGET)
	@echo

##################################
# [Compile] Compile Part
# Group1 소스(*.cpp)들에 대해, 대상경로($(@D))를 생성하고, dependency 파일을 생성한뒤 컴파일 한다.
$(GROUP.01.OBJ): $(BUILD_DIR)/%.o: %.cpp
	@echo ----------------------------------------
	@echo Compile : [GROUP.01] $<
	@echo ----------------------------------------
	@test -d $(@D) || mkdir -p $(@D)
	$(CC) -MM -MF $(DEPEND_FILE) -MT"$(DEPEND_FILE:.d=.o)" $(CFLAGS) $<
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo

$(GROUP.02.OBJ): $(BUILD_DIR)/%.o: %.c
	@echo ----------------------------------------
	@echo Compile : [GROUP.02] $<
	@echo ----------------------------------------
	@test -d $(@D) || mkdir -p $(@D)
	$(CC) -MM -MF $(DEPEND_FILE) -MT"$(DEPEND_FILE:.d=.o)" $(CFLAGS) $<
	$(CC) $(CFLAGS) -DSPECIAL_DEFINE -c -o $@ $<
	@echo
	
##################################
# Build가 시작되었다.
# 필요한 전처리가 있다면 구현한다.
PRE_BUILD:
	@echo ========================================
	@echo Make file started [config =\> $(CONFIG)]
	@echo ========================================
	@echo 

##################################
# Build가 종료되었다.
# 필요한 후처리가 있다면 구현한다.
POST_BUILD:
	@echo ========================================
	@echo Make file finished [config =\> $(CONFIG)]
	@echo ========================================

##################################
# Clean up 한다.
clean:
	rm -f $(GROUP.01.OBJ)
	rm -f $(GROUP.01.DEP)
	rm -f $(GROUP.02.OBJ)
	rm -f $(GROUP.02.DEP)
	rm -f $(TARGET)
	@echo ----------------------------------------
	@echo Clean work finished [config =\> $(CONFIG)]
	@echo ----------------------------------------

##################################
# 끝 부분에 dependency 파일들을 include한다.
-include $(GROUP.01.DEP)
-include $(GROUP.02.DEP)

##########################################################
# document
##########################################################
#
# Group 추가 요령
#   - 추가한다는 것은 다른 Group과 비교하여 서로간에,
#       ; 빌드 환경이 다른 경우 (컴파일 옵션, 빌드 경로, 소스 확장자, ...) 
# 가 발생하였기 때문이다.
#
# 1. Group 정의 추가
#
# Group 선언부에 아래 부분을 추가한다.
# 만일 여러 파일이 동일한 sub directory에 있는 경우 addprefix를 이용할 수 있다.
#
# ##################################
# # Group 03
# GROUP.03.SRC = G1.cpp G2.cpp
# GROUP.03.SRC.TMP = G3.cpp G4.cpp G5.cpp G6.cpp G7.cpp
# GROUP.03.SRC += $(addprefix ./some_group/, $(GROUP.03.SRC.TMP))
# GROUP.03.OBJ = $(addprefix $(BUILD_DIR)/,$(GROUP.03.SRC:.cpp=.o))
# GROUP.03.DEP = $(GROUP.03.OBJ:.o=.d)
#
# 2. Link 추가
#
# Target에 아래와 같이 GROUP.03.OBJ를 depend하여 Link되도록 한다.
#
# $(TARGET): $(GROUP.01.OBJ) $(GROUP.02.OBJ) $(GROUP.03.OBJ)
#
# 3. Compile 추가
#
# 아래와 같은 규칙을 추가한다. 확장자나 컴파일 옵션등을 따로 설정할 수 있다.
#
# $(GROUP.03.DEP): $(BUILD_DIR)/%.d: %.cpp
#	@echo ----------------------------------------
#	@echo Depend : $<
#	@echo ----------------------------------------
#	@test -d $(@D) || mkdir -p $(@D)
#	$(CC) -MM -MF $(DEPEND_FILE) -MT"$(DEPEND_FILE:.d=.o)" $<
#	$(CC) $(CFLAGS) -c -o $@ $<
#	@echo 
#
# 4. clean 추가
#
#  	rm -f $(GROUP.03.OBJ)
# 	rm -f $(GROUP.03.DEP)
#
# 5. include 추가
#
# -include $(GROUP.02.DEP)
#
# * 지원
# make
# make debug
# make release
# make clean [release, debug]
# make rebuild [release, debug]
#
# (EOF)