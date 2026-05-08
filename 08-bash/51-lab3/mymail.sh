#!/bin/bash

template="mtemplate.txt"
usernames=$(awk -F: '$3 >= 1000 {print $1}' /etc/passwd )

for username in $usernames;
do
	echo "Sending mail to $username"
	mail $username < $template
done
