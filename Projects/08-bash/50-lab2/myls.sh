#!/bin/bash

if [ $1  == "-l" ]
then
	echo "List in long format"
elif [ $1 == "-a" ]
then
	echo "List all entries including the hiding files"
elif [ $1 == "-d" ]
then
	echo "if an argument is a directory list only its name"
elif [ $1 == "-i" ]
then
	echo "print inode number"
elif [ $1 == "-R" ]
then
	echo "recursively list subdirectories"
fi
