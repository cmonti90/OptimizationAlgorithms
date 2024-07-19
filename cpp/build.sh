#!/bin/sh

# Build the project

buildir="build"

if [ ! -d "$buildir" ]; then
    mkdir $buildir
fi

cmake -S . -B $buildir

cmake --build $buildir