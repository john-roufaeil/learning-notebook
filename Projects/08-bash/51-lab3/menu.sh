#!/bin/bash

menu=("Press 1 to ls" "Press 2 to ls -a" "Press 3 to exit")

# select var in "${menu[@]}"
# do
# case $REPLY in
#   1) ls ;;
#   2) ls -a ;;
#   3) exit ;;
#   *) echo "Invalid option" ;;
# esac
# done

while true; do
for i in "${!menu[@]}"; do
  echo "$((i+1))) ${menu[i]}"
done

read -p "Enter choice: " choice

case $choice in
  1) ls ;;
  2) ls -a ;;
  3) exit ;;
  *) echo "Invalid option" ;;
esac
done