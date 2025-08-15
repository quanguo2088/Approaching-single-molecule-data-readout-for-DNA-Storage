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

./get_pn_seq ../data/plasmid_sequence.txt
./minimap2 -d ../data/pn_sequence.mmi ../data/pn_sequence.fa 2> /dev/null
./get_reads $fqindex $readCoverage $fqfile
