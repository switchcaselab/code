#!/bin/bash

OBJS=()

for arg in "$@"; do
    name=$(basename $arg)
    as -o $PWD/Bin/$name.o $arg    
    OBJS+=("$PWD/Bin/$name.o")
done

name=$(basename $1)
ld -o $PWD/Bin/$name.e ${OBJS[@]}
$PWD/Bin/$name.e

echo "Exit Code: $?" 