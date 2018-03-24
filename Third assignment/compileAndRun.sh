#!/bin/bash
if [[ -f ex3 ]];then
    rm ex3
fi
g++ -o ex3 *.cpp -std=c++11 -Wall -Wno-vla -pedantic -march=core2 -Os -pipe -fstack-protector-all -g3 -Wl,-O,1,-z,combreloc
./ex3 15 6 2
echo