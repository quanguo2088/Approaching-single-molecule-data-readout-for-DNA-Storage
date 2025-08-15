#!/bin/bash

export LD_LIBRARY_PATH=../configure/lib:$LD_LIBRARY_PATH

# Set variables for file paths and parameters
orig_cw="../data/original_codeword.txt"
decoding_res="../results/information.txt"
check_file="../results/Check.txt"
error_file="../results/bit_error_befor_decoding.txt"

readCoverage=2      
readNumber=1       # [0-250]

polished_cw="../results/polished_codeword.txt"
consensus_cw="../results/${readCoverage}x/consensus_codeword.txt"
consensus_cw_soft="../results/${readCoverage}x/consensus_codeword_soft.txt"

cd ./src

# 3.1 Bit-wise majority voting
./MergeCodeword "$polished_cw" "$consensus_cw" 1
decodestatus=$?
if [ $decodestatus -eq 0 ]; then

# 3.2 Perform decoding
./to_soft_inf "$consensus_cw" "$consensus_cw_soft"
./R093_decode "$consensus_cw_soft" "../results/decode_data_output.txt" > "../results/decoding_info_${readCoverage}x.txt"

first_line=$(head -n 1 "../results/decoding_info_${readCoverage}x.txt")
match=$(echo "$first_line" | grep -oP 'check\s*=\s*\K.*')
echo "$match" > "../results/Check.txt"

fi
