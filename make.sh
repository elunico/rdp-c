#!/bin/zsh

# optionally clean build files

if [[ "$1" == "clean" || "$2" == "clean" ]]; then
  echo "Cleaning build files"
  # if [ -d rdp-c.dSYM ] || [ -d rdp-cpp.dSYM ]; then
  #   rm -rf *.dSYM
  # fi
  # rm ./rdp-c ./rdp-cpp
  rm -rf build/
fi

if ! [ -d build/ ]; then
  mkdir build
fi

DEBUG_FLAG=""

# optionally setting debug flags

if [[ "$1" == "debug" || "$2" == "debug" ]]; then
  echo "Debugging true"
  DEBUG_FLAG="-g"
fi

# build c version

echo "Compiling C Version"
cc -std=gnu11 -o build/rdp-c -Iinclude/ $DEBUG_FLAG src/*.c

# build c++ version

echo "Compiling C++ Version"
c++ -std=c++20 -o build/rdp-cpp -Iinclude/ $DEBUG_FLAG src/*.cpp
