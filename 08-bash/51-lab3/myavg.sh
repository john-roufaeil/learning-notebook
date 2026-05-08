#!/bin/bash

read -p "Enter your numbers separated by spaces: " -a nums
sum=0
for num in ${nums[@]};
do
	sum=$((sum+num))
done

avg=$((sum/${#nums[@]}))
echo "Avg of your entered numbers is $avg"
