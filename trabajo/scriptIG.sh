#!/bin/bash

cd cmake-unix/build/

if [ -n "$1" ]; then
    make clean
else
    make
fi

make

cd ..
cd ..

cmake-unix/build/pracs_ig_exe


