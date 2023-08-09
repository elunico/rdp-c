.PHONY: all debug clean fresh clang gcc

.DEFAULT_GOAL := all

SRCS := $(wildcard *.c)
INCLUDES = include/

all: cver cppver

CC = cc
CXX = c++

CCFLAGS = -std=gnu11
CXXFLAGS = -std=c++20

CLIBRARY = -lm
CXXLIBRARY = -lm

clang: CC = clang
clang: CXX = clang++
clang: all

gcc: CC = gcc
gcc: CXX = g++
gcc: all

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: all

cver %.c %.h:
	${CC} -o build/rdp-c -I${INCLUDES} ${CLIBRARY} ${CCFLAGS} src/*.c

cppver %.cpp %.hpp:
	${CXX} -o build/rdp-cpp -I${INCLUDES} ${CXXLIBRARY} ${CXXFLAGS} src/*.cpp

fresh: clean cver cppver

clean:
	rm -rf build/
	mkdir build/
