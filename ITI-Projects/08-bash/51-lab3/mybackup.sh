#!/bin/bash

files=$(ls $HOME)

for file in $files;
do
	if [ -f $HOME/$file ];
	then
		echo "backing up file $HOME/$file"
		cp $HOME/$file $HOME/$file.bk
	fi
done


