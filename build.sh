#!/bin/bash

# rm -rf _build/
rm -rf imgui.ini
cmake . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build 

# Compilar para Windows (cross-compilation)
#cmake . -B _build_windows -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake -DCMAKE_BUILD_TYPE=Release
#cmake --build _build_windows
