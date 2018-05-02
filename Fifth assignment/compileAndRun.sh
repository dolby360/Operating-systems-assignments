#!/bin/bash
if [[ -f multi-lookup ]];then
    rm multi-lookup
fi

g++ -o multi-lookup *.cpp -std=c++11 -Wall -Wno-vla -pedantic -march=core2 -Os -pipe -fstack-protector-all -g3 -pthread -Wl,-O,1,-z,combreloc,-rpath=. -lmyutil -L .

if [[ -f multi-lookup ]];then
./multi-lookup
fi
echo
