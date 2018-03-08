#!/bin/bash

if [ ! -d ../backup ];then
	mkdir ../backup
fi

for iter in *; do
	if [ -f "$iter" ];then
		cp "$iter" ../backup/"$iter"
	fi

	if [ -d "$iter" ];then
		if [ ! -d ../backup/"$iter" ];then
			mkdir ../backup/"$iter"
		fi
		for interIter in $iter/* ;do
			cp "$interIter" ../backup/"$interIter"
		done
	fi

done