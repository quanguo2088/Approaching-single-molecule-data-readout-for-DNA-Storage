#!/bin/bash



fastqname="../data/pLP3_pLP4.fastq"

processnum=3
erasureThreshold=0.1


if [ -d "./results/" ]; then
    rm -rf "./results/"
fi
mkdir -p "./results/$processnum"


./s0_compile.sh


start_time=$(date '+%Y-%m-%d %H:%M:%S')
echo "Start Time: $start_time"

fqindex=0
while true; do

    if [ -z "$start_time" ]; then
        start_time=$(date '+%Y-%m-%d %H:%M:%S')
        echo "Start Time: $start_time"
    fi

    ./s1_get_pn_seq_and_reads.sh "$fastqname" $processnum $fqindex $erasureThreshold
    ./s2_alignment_and_correction.sh "$fastqname" $processnum $fqindex $erasureThreshold
    ./s3_decoding.sh "$fastqname" $processnum $fqindex $erasureThreshold
    ./s4_recovery.sh "$fastqname" $processnum $fqindex $erasureThreshold
    fqindex=$((fqindex + 3))


    if [ -f "./results/check.txt" ]; then
        result=$(cat "./results/check.txt")
        if [ $result -eq 1 ]; then
            break
        fi
    fi

done


end_time=$(date '+%Y-%m-%d %H:%M:%S')
echo ""
echo "End Time: $end_time"

start_timestamp=$(date -d "$start_time" +%s)
end_timestamp=$(date -d "$end_time" +%s)
time_difference=$((end_timestamp - start_timestamp))

echo "Elapsed time: $time_difference seconds"