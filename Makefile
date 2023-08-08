.PHONY: all debug clean fresh

.DEFAULT_GOAL := all

SRCS := $(wildcard *.c)
INCLUDES = include/

all: cver cppver

CCFLAGS = -std=gnu11
CXXFLAGS = -std=c++20

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: all

cver %.c %.h:
	cc -std=gnu11 -o build/rdp-c -I${INCLUDES} ${CCFLAGS} src/*.c

cppver %.cpp %.hpp:
	c++ -std=c++20 -o build/rdp-cpp -I${INCLUDES} ${CXXFLAGS} src/*.cpp

fresh: clean cver cppver

clean:
	rm -rf build/
	mkdir build/
