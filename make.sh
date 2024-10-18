#!/bin/sh

rm -f ./build/Makefile
cmake -S . -B build -G "Unix Makefiles"

cd ./build
make
