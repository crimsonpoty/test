TARGET = fc

CXX = g++
CXX_FLAGS = -W -Wall -g
CXX_FLAGS += -std=c++11
LD_FLAGS = 

SRCS = file_cutter.cpp


.PHONY : all clean

all: clean $(TARGET)

$(TARGET):
	@echo "[Compile... $@]"
	@$(CXX) -o $(TARGET) $(SRCS) $(CXX_FLAGS) $(LD_FLAGS)

clean:
	@rm -f $(TARGET)
