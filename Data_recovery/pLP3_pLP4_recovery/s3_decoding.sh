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
a=$(( fqindex / 3 + 1 ))

./MergeCodeword1 "../results/polished_codeword1.txt" "../results/$readCoverage/consensus_codeword1.txt" $a 1
decodestatus1=$?

./MergeCodeword2 "../results/polished_codeword2.txt" "../results/$readCoverage/consensus_codeword2.txt" $a 1
decodestatus2=$?

if [ $decodestatus1 -eq 0 ] && [ $decodestatus2 -eq 0 ]; then

    first_line_file1=$(head -n 1 "../results/$readCoverage/consensus_codeword1.txt")
    first_line_file2=$(head -n 1 "../results/$readCoverage/consensus_codeword2.txt")
    echo -e "${first_line_file1}${first_line_file2}" > "../results/consensus_codeword.txt"
    ./MergeCodeword3 $threshold
    decodestatus3=$?

    if [ $decodestatus3 -eq 0 ]; then

        echo -e "\n+++++++++++++++++++++ Starting LDPC decoding ++++++++++++++++++++"
        ./NBLDPC_decoder "../results/consensus_codeword.txt" "../results/information.txt"
        ./readtxt "../results/check.txt"

    fi

fi
