#!/bin/bash

old_lines_count=$(wc -l < /var/mail/$USER)

while true; do
	sleep 10
	new_lines_count=$(wc -l < /var/mail/$USER)

	if [ "$new_lines_count" -gt "$old_lines_count" ]; then
		echo "new mail"
		old_lines_count=$new_lines_count
	fi
done
