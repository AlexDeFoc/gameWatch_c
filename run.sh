#!/bin/sh

echo "Building..."
cd build
ninja

echo "Running..."
./app.exe
