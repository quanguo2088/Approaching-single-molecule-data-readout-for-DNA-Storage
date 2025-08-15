#!/bin/bash

# Compile the LDPC decoder
# - Output binary: bin/R093_decode
# - Source: src/R093_decode.cpp + all .cpp files in configure/LDPC
# - Include headers from configure/LDPC
g++ -o bin/R093_decode src/R093_decode.cpp -I configure/LDPC -L configure/lib -lldpc

# Compile the recovery program to reconstruct Shakespeare's sonnets
# - Output binary: bin/recovery_sonnets
# - Source: src/recovery_sonnets.c
gcc -o bin/recovery_sonnets src/recovery_sonnets.c

# Step 1: Run LDPC decoding
# - Input: soft decision probabilities (softinfo_prob.txt)
# - Output: decoded bitstream (decode_output.txt)
./bin/R093_decode data/softinfo_prob.txt results/decode_output.txt

# Step 2: Recover original text from decoded bitstream
# - Input: decoded bitstream (decode_output.txt)
# - Output: recovered Shakespeare's sonnets (recovered_shakespeare_sonnets.txt)
./bin/recovery_sonnets results/decode_output.txt results/recovered_shakespeare_sonnets.txt

cat results/recovered_shakespeare_sonnets.txt