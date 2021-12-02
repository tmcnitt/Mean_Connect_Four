CC := g++
CFLAGS :=  -g --std=c++11 -O3 
TARGET := solver

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard *.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
clean:
	rm -rf $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)

depend: .depend

move: clean
	scp -r ../PNS_ConnectFour tmcnitt@genuse61.smu.edu:.

.depend: $(SRCS)
	rm -f "$@"
	$(CC) $(CFLAGS) -MM $^ -MF "$@"

include .depend

.PHONY: all clean