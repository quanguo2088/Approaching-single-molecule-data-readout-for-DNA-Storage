#!/bin/bash

export LD_LIBRARY_PATH=./configure/lib:$LD_LIBRARY_PATH

# Compile the R093_encode program
# - Output binary to bin/R093_encode
# - Source code: src/R093_encode.cpp and all .cpp files in configure/LDPC
# - Include header files from LDPC directory
g++ -o bin/R093_encode src/R093_encode.cpp -I configure/LDPC -L configure/lib -lldpc

# Run the encoder
# - Input: Shakespeare_sonnets.txt (raw text)
# - Output: enc_base_seq_32000nt.txt (encoded base sequence, 32000 nt)
./bin/R093_encode data/Shakespeare_sonnets.txt results/enc_base_seq_32000nt.txt
