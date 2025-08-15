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

./minimap2 -a "../data/pn_sequence.mmi" "../results/seq.fastq" > "../results/temp.sam" 2> /dev/null

./indel_correction "../data/pn_sequence.fa" "../results/temp.sam" $fqindex $readCoverage $fqfile