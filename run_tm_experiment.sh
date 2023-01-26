#!/usr/bin/bash

make tm_experiments

# Example: ./tm_experiments f=./filters_original/acl1.txt
FILTERS_DIR="./filters_original"
for FILE in $(ls "$FILTERS_DIR"); do
  ./tm_experiments f="$FILTERS_DIR/$FILE";
done;