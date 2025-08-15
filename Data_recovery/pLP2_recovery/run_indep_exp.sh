#!/bin/bash

./s0_compile.sh

# Set variables for file paths and parameters
fqfile="../data/pLP2.fastq"
reffile="../data/Plasmid_sequence.txt"
orig_cw="../data/original_codeword.txt"

pn_seq="../data/PN_sequence.fa"
pn_mmi="../data/PN_sequence.mmi"
filtered_fq="../data/High_quality.fastq"
corrupted_pn="../data/Corrupted_PN_sequence.fastq"
align_sam="../results/Minimap_align.sam"
polished_cw="../results/polished_codeword.txt"
consensus_cw="../results/consensus_codeword.txt"

error_file="../results/bit_error_befor_decoding.txt"
check_file="../results/Check.txt"
decoding_res="../results/information.txt"


# Ensure required directories are clean and created
if [ -d "./results/" ]; then
    rm -rf "./results/"
fi
mkdir -p "./results/"

cd ./src
./get_pn_seq "$reffile" "$pn_seq"
./minimap2 -d "$pn_mmi" "$pn_seq" 2> /dev/null
./filter_by_length $fqfile "$filtered_fq"

readCoverage=2
for readNumber in $(seq 0 804)
do

./get_reads "$readNumber" "$readCoverage" "$filtered_fq" "$corrupted_pn"
./minimap2 -a  "$pn_mmi" "$corrupted_pn" > "$align_sam" 2> /dev/null
./indel_correction "$pn_seq" "$align_sam" "$readNumber" "$readCoverage" "$filtered_fq"

./MergeCodeword "$polished_cw" "$consensus_cw" 1
rm -rf "$polished_cw"

decodestatus=$?
if [ $decodestatus -eq 0 ]; then
./R13Decoder "$consensus_cw" "$orig_cw" "$decoding_res" 3.0 1 200 >> "../results/decoding_info.txt"
fi

done