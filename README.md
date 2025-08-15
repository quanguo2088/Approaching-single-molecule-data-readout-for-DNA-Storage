# Approaching single-molecule data readout for DNA storage

![tu1](pic/1.png)

## Table of Contents

- [Approaching single-molecule data readout for DNA storage](#approaching-single-molecule-data-readout-for-dna-storage)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Requirements](#requirements)
  - [Overview of Repository Files](#overview-of-repository-files)
  - [Example of usage](#example-of-usage)
    - [1. PNC-LDPC coding scheme](#1-pnc-ldpc-coding-scheme)
      - [Encoding](#encoding)
      - [Decoding](#decoding)
    - [2. Data Readout](#2-data-readout)
      - [1. Get PN sequence and reads](#1-get-pn-sequence-and-reads)
      - [2. PN sequence alignment \& indel correction](#2-pn-sequence-alignment--indel-correction)
      - [3. LDPC decoding](#3-ldpc-decoding)
      - [4.  Recovery of original file](#4--recovery-of-original-file)
  - [License](#license)

## Overview

Synthetic DNA is a promising medium for long-term data storage. Nanopore sequencing can facilitate rapid readout of large DNA fragments, though suffering from the severe and notorious insertion/deletion errors. We present a highly reliable storage scheme with medium-length encoded DNA fragments (a few to tens of kilobases) and fast recovery schemes using low coverage nanopore sequencing. Using the readout-aware pseudo-noise sequences, the nanopore reads with arbitrary start points are directly located and the base insertions/deletions are identified, enabling fast and reliable recovery even at very low coverages.

Here we provide the code for the **PNC-LDPC coding scheme** and **data readout**.

**1. PNC-LDPC Coding Scheme**
The PNC-LDPC module provides the following functions:

- **Encoding** — Encode original data (3,716 bytes) to construct a 43-kb plasmid, using a binary LDPC code with a code rate of 0.93 (by MacKay, D. J. C., available at [http://www.inference.org.uk/mackay/codes/data.html#l142](http://www.inference.org.uk/mackay/codes/data.html#l142)).
- **Decoding** — Reconstruct the original data from soft-decision information.

**2. Data Readout**
The data readout process consists of the following steps:

- **Get PN Sequence and Reads** — Extract corrupted PN sequences from nanopore sequencing reads.
- **PN Sequence Alignment & Indel Correction** — Align the corrupted PN sequence to the ideal PN sequence and perform insertion/deletion (indel) correction.
- **LDPC Decoding** — Apply non-binary LDPC decoding. For a detailed explanation of the coding procedures, see *Encoder Implementation with FPGA for Non-Binary LDPC Codes*, in *2012 18th Asia-Pacific Conference on Communications (APCC)*, DOI: [10.1109/APCC.2012.6388230](https://doi.org/10.1109/APCC.2012.6388230).
- **Recovery of Original File** — Reconstruct the original data from the decoded bitstream.

To demonstrate the feasibility of the proposed method, we constructed 5 plasmids with lengths ranging from 33 to 43 kb and 28 plasmids with lengths ranging from 6 to 8 kb. We provide the raw data (poems in TXT format), the encoded DNA sequences, and sequencing reads obtained via efficient library preparation and nanopore sequencing. All programs are developed in C and C++.

## Requirements

**OS Requirements**

The program has been tested on the following operating systems:

- **Ubuntu 20.04.6 LTS**
- **Ubuntu 18.04.6 LTS**

**Software Requirements**

The following tools and dependencies are required:

- **C/C++ Compiler**: Ensure `gcc` and `g++` is installed.
- **Minimap2**: The Minimap2 binary should be available.  You can download it from the official repository: [https://github.com/lh3/minimap2](https://github.com/lh3/minimap2).

## Overview of Repository Files

```plaintext
PNC-LDPC_Encoding/
├── configure/
│   ├── LDPC/                        # Configuration files
│   ├── lib/                         # Supporting libraries
│   ├── PNseq_32000                  # Pseudo-noise sequence (length 32,000)
│   ├── R093.gen                     # LDPC generator matrix (R=0.93)
│   └── R093.pchk                    # LDPC parity-check matrix (R=0.93)
│
├── data/
│   ├── Shakespeare_sonnets.txt      # Source text data (Shakespeare's Sonnets)
│   └── softinfo_prob.txt            # Soft information probabilities for decoding
│
└── src/
    ├── R093_decode.cpp              # LDPC decoding implementation (R=0.93)
    ├── R093_encode.cpp              # LDPC encoding implementation (R=0.93)
    └── recovery_sonnets.c           # Recover original sonnets from decoding result
                                            
Data_recovery/
├── data/
│   ├── original_codeword.txt       # 22,680-bit codeword encoded from poems.txt using non-binary LDPC (22680, 7560) code.
│   ├── pseudo_noise_sequence.txt   # PN sequence for locating reads & indel correction
│   ├── plasmid_sequence.txt        # DNA plasmid sequence (33,558 bp)
│   ├── data_position.txt           # Start/end positions of encoded DNA in plasmid
│   ├── poems.txt                   # Original data with 7 Chinese poems (945 bytes)
│   └── pLP2.fastq                  # Sample data (5,631 sequencing reads, zipped)
│
└── src/
    ├── get_pn_seq.c                # Map PN sequence to base sequence {0 → A, 1 → T}
    ├── filter_by_length.c          # Filter highly matched reads
    ├── get_reads.c                 # Map corrupted PN sequence to base sequence
    ├── indel_correction.c          # Detect & correct indel errors in raw reads
    ├── R13Decoder                  # Implements (22680, 7560) R=1/3 non-binary LDPC decoding.
    ├── parse_decoding_result.c     # Parse and process decoding results
    ├── recovery_poem.c             # Recover original text from decoding result
    └── merge_codeword.c            # Bit-wise majority voting
```

## Example of usage

### 1. PNC-LDPC coding scheme

**Compilation**

```bash
cd PNC-LDPC_Encoding/
export LD_LIBRARY_PATH=./configure/lib:$LD_LIBRARY_PATH
g++ -o bin/R093_encode src/R093_encode.cpp -I configure/LDPC -L configure/lib -lldpc
g++ -o bin/R093_decode src/R093_decode.cpp -I configure/LDPC -L configure/lib -lldpc
```

#### Encoding

```bash
./bin/R093_encode data/Shakespeare_sonnets.txt results/enc_base_seq_32000nt.txt
```

**Input files:**

- **Shakespeare_sonnets.txt**: original text file to be encoded.

**Output files:**

- **enc_base_seq_32000nt.txt**: encoded base sequence, 32000 nt.

#### Decoding

```bash
./bin/R093_decode data/softinfo_prob.txt results/decode_output.txt
./bin/recovery_sonnets results/decode_output.txt results/recovered_shakespeare_sonnets.txt
```

**Input files:**

- **softinfo_prob.txt**: soft decision probabilities.

**Output files:**

- **decode_output.txt**: decoded bitstream.
- **recovered_shakespeare_sonnets.txt**: recovered Shakespeare's sonnets.

### 2. Data Readout

**Compilation**

To compile the individual modules:

```bash
cd Data_recovery/
gcc -o filter_by_length filter_by_length.c
gcc -o get_pn_seq get_pn_seq.c
gcc -o get_reads get_reads.c
gcc -o indel_correction indel_correction.c -lm
gcc -o MergeCodeword merge_codeword.c
gcc -o parse_decoding_result parse_decoding_result.c -lm
gcc -o recovery_poem recovery_poem.c
```

- The **run_pipeline.sh** script integrates four steps and achieves data recovery using a single read, with a total execution time of approximately 3 seconds.
- The **run_indep_exp.sh** script can perform recovery tests under different sequencing coverages.

#### 1. Get PN sequence and reads

```bash
./run_get_pn_seq_and_reads.sh
```

**Input files:**

- **Plasmid_sequence.txt**: DNA sequence of a plasmid with a total length of 33,558 base pairs (bp).
- **pLP2.fastq**: Nanopore reads obtained from efficient library preparation.

**Output files:**

- **PN_sequence.fa**: Pseudo-noise (PN) sequence mapped to a base sequence using the rule `{0 → A, 1 → T}`.
- **PN_sequence.mmi**: Minimap2 generated index for the PN sequence.
- **High_quality.fastq**: Retrieved high-quality plasmid nanopore reads.
- **Corrupted_PN_sequence.fastq**: Corrupted PN sequences derived from the nanopore reads.

#### 2. PN sequence alignment & indel correction

```bash
./run_alignment_and_correction.sh
```

**Input files:**

- **PN_sequence.mmi**: Minimap2 index file for the original PN sequence, generated in the step 1.
- **Corrupted_PN_sequence.fastq**: Corrupted PN sequence, generated in the step 1.
- **PN_sequence.fa**: Original PN sequence, generated in the step 1.

**Output files:**

- **Minimap_align.sam**: Alignment of the PN sequence to the corrupted sequences.
- **polished_codeword.txt**: The corrected codeword after alignment and indel correction.
- **bit_error_befor_decoding.txt**: This file contains four columns: error count, erasure count, error rate, and erasure rate, representing bit error characteristics after indel correction.

#### 3. LDPC decoding

```bash
./run_decoding.sh
```

**Input files:**

- **polished_codeword.txt**: Codeword file generated from the step 2.
- **Original_codeword.txt**: The original encoded codeword for error rate analysis.

**Output files:**

- **Information.txt**: Decoded information from the codeword.
- **Check.txt**: This file is used to verify the accuracy of decoding results.

#### 4.  Recovery of original file

```bash
./run_recovery.sh
```

**Input files:**

- **Check.txt**: Used to validate decoding results generated in the step 3.

**Output files:**

- **Poetry_of_recovery.txt**: Digital file recovered from the decoding result. In this example, the stored digital file contains 7 Chinese poems.

## License

This project is distributed under the MIT License, which allows for both commercial and private use, modification, and distribution. See the LICENSE file in the repository for more detailed information.
