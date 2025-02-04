#!/bin/bash

# rm -rf _build/
rm -rf imgui.ini
cmake . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build 
