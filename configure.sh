#!/bin/bash

echo "Configuring gameWatch..."

cmake -S . -G "Ninja" .. -B "./build" -DCMAKE_BUILD_TYPE=Debug

echo "Building gameWatch..."
cd build
ninja
