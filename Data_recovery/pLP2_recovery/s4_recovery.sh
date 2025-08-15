#!/bin/bash


poem_recovery="../results/Poetry_of_recovery.txt"
check_file="../results/Check.txt"

cd ./src

# Check if recovery was successful
result=$(< "$check_file")
if [ "$result" == "0" ]; then
    echo "Recovery failed."
    exit 1
else
    echo "Recovery succeeded."
    ./recovery_poem
    echo ""
    cat "$poem_recovery"
    echo ""
fi

