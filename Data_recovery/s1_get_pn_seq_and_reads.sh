#!/bin/bash

# Set variables for file paths and parameters
fqfile="../data/pLP2.fastq"
reffile="../data/Plasmid_sequence.txt"

pn_seq="../data/PN_sequence.fa"
pn_mmi="../data/PN_sequence.mmi"
filtered_fq="../data/High_quality.fastq"
corrupted_pn="../data/Corrupted_PN_sequence.fastq"

readCoverage=1      
readNumber=1       # [0-1609]

# Ensure required directories are clean and created
if [ -d "./results/" ]; then
    rm -rf "./results/"
fi

# Now create the necessary directories
mkdir -p "./results/${readCoverage}x"


# First-time setup: Extract the example sequencing reads from the ZIP archive
if [ ! -f "data/pLP2.fastq" ]; then
    unzip data/pLP2.zip -d data/
else
    echo "File data/pLP2.fastq already exists. Skipping extraction."
fi


cd ./src

# 1.1 Read the PN sequence
./get_pn_seq "$reffile" "$pn_seq"

# 1.2 Generate an index using Minimap2
./minimap2 -d "$pn_mmi" "$pn_seq" 2> /dev/null

# 1.3 Filter reads with large differences
./filter_by_length $fqfile "$filtered_fq"

# 1.4 Extract reads from the filtered FASTQ file
./get_reads "$readNumber" "$readCoverage" "$filtered_fq" "$corrupted_pn"




