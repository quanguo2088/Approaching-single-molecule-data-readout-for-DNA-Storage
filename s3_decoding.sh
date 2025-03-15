#!/bin/bash

# Set variables for file paths and parameters
orig_cw="../data/original_codeword.txt"
decoding_res="../results/information.txt"
check_file="../results/Check.txt"
error_file="../results/bit_error_befor_decoding.txt"

readCoverage=1      
readNumber=1  # [0-1609]

polished_cw="../results/polished_codeword.txt"
consensus_cw="../results/${readCoverage}x/consensus_codeword.txt"

cd ./src

# 3.1 Bit-wise majority voting
./MergeCodeword "$polished_cw" "$consensus_cw" 1
decodestatus=$?
if [ $decodestatus -eq 0 ]; then

# 3.2 Perform decoding
./R13Decoder "$consensus_cw" "$orig_cw" "$decoding_res" 3.0 1 200 > "../results/decoding_info_${readCoverage}x.txt"

# 3.3 Calculate bit error rate after decoding
./parse_decoding_result "../results/decoding_info_${readCoverage}x.txt" "$check_file"

fi
