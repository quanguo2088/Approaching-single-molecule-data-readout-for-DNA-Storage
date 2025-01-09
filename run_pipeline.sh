#!/bin/bash

./s0_compile.sh

# s1
./s1_get_pn_seq_and_reads.sh

# s2
./s2_alignment_and_correction.sh

# s3
./s3_decoding.sh

# s4
./s4_recovery.sh
