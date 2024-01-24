#!/bin/sh

# autotools preset makefiles
autoreconf --install

# create a build directory
mkdir build
cd build
../configure --enable-gcov
cd -

# create a debug deirectory, like build but add debug flags
mkdir debug
cd debug
../configure CFLAGS='-fsanitize=address' --enable-debug
cd -
