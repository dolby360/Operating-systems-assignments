#!/bin/bash
cat "$1" | tr [:upper:] [:lower:] | grep -Eo "\<[a-z1-9]{3,8}\>" | sort -u -d >"$1".dict