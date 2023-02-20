SRC = src/main.cpp
TARGET = montFib.exe

CXX = g++
CXXFLAGS = 
LDFLAGS = 
LIBS = 

all: $(TARGET)

$(TARGET): 
	$(CXX) $(SRC) -o $@

.PHONY: clean

clean:
	-rm $(TARGET) $(SRC:%.c=%.o)

