#!/bin/bash

# Compile C programs
cd ./src
gcc -o filter_by_length filter_by_length.c
gcc -o get_pn_seq get_pn_seq.c
gcc -o get_reads get_reads.c
gcc -o indel_correction indel_correction.c -lm
gcc -o MergeCodeword merge_codeword.c
gcc -o parse_decoding_result parse_decoding_result.c -lm
gcc -o recovery_sonnets recovery_sonnets.c
gcc -o to_soft_inf to_soft_inf.c 