#!/bin/bash

# Compile C programs
cd ./src
gcc -o filter_by_length filter_by_length.c
gcc -o get_pn_seq get_pn_seq.c
gcc -o get_reads get_reads.c
g++ -o indel_correction indel_correction.c edlib.cpp
gcc -o MergeCodeword merge_codeword.c
gcc -o parse_decoding_result parse_decoding_result.c -lm
gcc -o recovery_poem recovery_poem.c
