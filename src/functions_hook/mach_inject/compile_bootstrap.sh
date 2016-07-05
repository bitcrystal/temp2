#!/bin/bash
gcc -c bootstrap.c -o lbootstrap.o
dylibc ./ bootstrap
mv ./libbootstrap.dylib ./bootstrap.dylib
rm ./lbootstrap.o
