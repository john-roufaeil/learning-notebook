#!/bin/bash

for num in "$@"; do
	echo "Square of $num is $((num*num))"
done;
