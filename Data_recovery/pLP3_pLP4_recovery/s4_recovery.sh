#!/bin/bash

if [ "$#" -ne 4 ]; then
    echo "Usage: $0 <fqfile> <readCoverage> <fqindex> <threshold>"
    exit 1
fi


fqfile="$1"
readCoverage="$2"
fqindex="$3"
threshold="$4"

cd ./src


result_file="../results/check.txt"


if [ ! -f "$result_file" ]; then
    exit 1
fi

result=$(< "$result_file")


if [ "$result" == "0" ]; then
    echo "Recovery failed."
else
    echo "Recovery succeeded."
    ./recovery_poem
    echo ""
    cat ../results/poem.txt
fi

