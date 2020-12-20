#!/bin/bash
svvap()
{
    local i=$1
    local j=$2
    local buf=${key[i]}
    key[i]=${key[j]}
    key[j]=$buf    
}
key=($@)           #
bound=$((${#@}-1)) # what is this?
while :; do
    t=0
    for ((j=0;j<bound;j++)); do
	if [ ${key[j]} -gt ${key[j+1]} ]; then
	    svvap $j $((j+1))
	    t=$j
	fi
    done
    [ $t -eq 0 ] && break
    bound=$t
done
echo ${key[*]}
