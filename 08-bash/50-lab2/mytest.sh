#!/bin/bash

if [ -d $1 ]
then
	echo "$1 is a directory"
else
	echo "$1 is a file"
fi

if [ -r $1 ]
then
	echo "$1 is readable"
fi

if [ -w $1 ]
then
	echo "$1 is writable"
fi

if [ -x $1 ]
then
	echo "$1 is executable"
fi
