#!/bin/sh

# autotools preset makefiles
autoreconf --install

# create a build directory
mkdir build
cd build
../configure
cd -

# create a debug deirectory, like build but add debug flags
mkdir debug
cd debug
../configure --enable-debug --enable-gcov
cd -
