#!/usr/bin/env bash

set -xe

CFLAGS="-Wall -Wextra $(pkg-config --cflags raylib)"
LIBS="$(pkg-config --libs raylib)"

clang $CFLAGS -o pong main.c $LIBS

# Check if the compilation was successful before running the executable
if [ $? -eq 0 ]; then
    ./pong
else
    echo "Compilation failed."
fi
