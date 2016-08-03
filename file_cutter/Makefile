TARGET = file_cutter

CXX = g++
CXX_FLAGS = -W -Wall -g
CXX_FLAGS += -std=c++11
LD_FLAGS = 


.PHONY : all clean pipe_viewer

all: clean $(TARGET)


$(TARGET):
	@echo "[Compile... $@]"
	@$(CXX) -o $@ $@.cpp $(CXX_FLAGS) $(LD_FLAGS)

clean:
	-@rm -f $(TARGET)

pipe_viewer:
	@echo "[Compile... $@]"
	@$(CXX) -o $@ $@.cpp $(CXX_FLAGS) $(LD_FLAGS)
