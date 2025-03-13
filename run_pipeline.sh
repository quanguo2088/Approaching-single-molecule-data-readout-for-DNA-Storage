#!/bin/bash

# First-time setup: Extract the example sequencing reads from the ZIP archive
if [ ! -f "data/pLP2.fastq" ]; then
    unzip data/pLP2.zip -d data/
else
    echo "File data/pLP2.fastq already exists. Skipping extraction."
fi


echo "Process Started:" `date`
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
start=$(date +%s.%3N)  

./s0_compile.sh

# s1
./s1_get_pn_seq_and_reads.sh

# s2
./s2_alignment_and_correction.sh

# s3
./s3_decoding.sh

# s4
./s4_recovery.sh

echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
end=$(date +%s.%3N)  
time=$(echo "$end - $start" | bc)  

echo "Process Completed:" `date`
echo "Total Execution Time: $time seconds"
