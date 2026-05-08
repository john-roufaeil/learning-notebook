#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Nothing is passed"

elif [ $# -eq 1 ]; then

case "$1" in
	[a-z])
		echo "$1 is lowercase";;
	[A-Z])
		echo "$1 is uppercase";;
	[0-9])
		echo "$1 is number";;
	*)
		echo "other character is passed";;
esac
fi
