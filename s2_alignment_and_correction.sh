#!/bin/bash

# Set variables for file paths and parameters
pn_seq="../data/PN_sequence.fa"
pn_mmi="../data/PN_sequence.mmi"
corrupted_pn="../data/Corrupted_PN_sequence.fastq"
align_sam="../results/Minimap_align.sam"
filtered_fq="../data/High_quality.fastq"

readCoverage=1      
readNumber=1       # [0-1609]

cd ./src

# 2.1 Align PN sequences with Minimap2
./minimap2 -a "$pn_mmi" "$corrupted_pn" > "$align_sam" 2> /dev/null

# 2.2 Perform indel correction
./indel_correction "$pn_seq" "$align_sam" "$readNumber" "$readCoverage" "$filtered_fq"


