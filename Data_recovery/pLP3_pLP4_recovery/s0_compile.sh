#!/bin/bash

# Compile C programs
cd ./src
gcc -o get_pn_seq get_pn_seq.c
gcc -o get_reads get_reads.c
g++ -o indel_correction indel_correction.cpp  -lm
gcc -o parse_decoding_result parse_decoding_result.c -lm
g++ -o recovery_poem recovery_poem.cpp
g++ -o MergeCodeword1 merge_codeword1.cpp
g++ -o MergeCodeword2 merge_codeword2.cpp
g++ -o MergeCodeword3 merge_codeword3.cpp
gcc -o readtxt readtxt.c
cd ..
