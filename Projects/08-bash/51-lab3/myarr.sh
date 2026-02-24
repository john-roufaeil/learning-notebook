#!/bin/bash

typeset -i n
typeset -a arr

read -p "Enter count of input: " n

for i in $(seq 0 $((n-1)));
do
	read -p "Enter element $((i+1)): " element
	arr[i]=$element
done

for e in ${arr[@]}
do
	echo $e
done
