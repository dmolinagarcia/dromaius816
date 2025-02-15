#!/bin/bash

# rm -rf _build/
rm -rf imgui.ini
cmake . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build 

# Compilar para Windows (cross-compilation)
cmake -B _  build_windows -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake
cmake --build _build_windows
