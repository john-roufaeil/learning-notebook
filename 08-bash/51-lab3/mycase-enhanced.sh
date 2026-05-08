#!/bin/bash

shopt -s extglob

if [ $# -eq 0 ]; then
	echo "Nothing is passed"
elif [ $# -eq 1 ]; then

case "$1" in
	+([a-z]))
		echo "String is all lowercases";;
	+([A-Z]))
		echo "String is all upercases";;
	+([0-9]))
		echo "String is all numbers";;
	+([a-zA-Z0-9]))
		echo "String is a mix";;
	*)
		echo "Sting is not valid";;
esac

fi
