.PHONY: all debug clean fresh

.DEFAULT_GOAL := all

SRCS := $(wildcard *.c)
INCLUDES = include/

all: cver cppver

CCFLAGS = -std=gnu11
CXXFLAGS = -std=c++20

CLIBRARY = -lm
CXXLIBRARY = -lm

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: all

cver %.c %.h:
	cc -o build/rdp-c -I${INCLUDES} ${CLIBRARY} ${CCFLAGS} src/*.c

cppver %.cpp %.hpp:
	c++ -o build/rdp-cpp -I${INCLUDES} ${CXXLIBRARY} ${CXXFLAGS} src/*.cpp

fresh: clean cver cppver

clean:
	rm -rf build/
	mkdir build/
