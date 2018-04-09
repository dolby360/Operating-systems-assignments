#!/bin/bash
if [[ -f shell  ]] 
then
    rm shell
fi
g++ -o shell *.cpp -std=c++11 -Wall -Wno-vla -pedantic -march=core2 -Os -pipe -fstack-protector-all -g3 -Wl,-O,1,-z,combreloc
if [[ -f shell  ]] 
then
    ./shell
fi

echo