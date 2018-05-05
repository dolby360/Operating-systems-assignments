#!/bin/bash
if [[ -f multi-lookup ]];then
    rm multi-lookup
fi

g++ -o multi-lookup *.cpp -std=c++11 -Wall -Wno-vla -pedantic -march=core2 -Os -pipe -fstack-protector-all -g3 -pthread -Wl,-O,1,-z,combreloc,-rpath=. -lmyutil -L .

if [[ -f multi-lookup ]];then
./multi-lookup input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt result.txt
fi
echo
