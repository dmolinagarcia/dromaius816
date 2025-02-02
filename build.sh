#!/bin/bash

rm -rf _build/
cmake . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build 