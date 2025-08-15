#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "ldpc_codec.h"

#define LDPC_K 29760
#define LDPC_N 32000

const char *enc_config_file = "./configure/R093.gen";
const char *ldpc_pchk_file  = "./configure/R093.pchk";
const char *watermark_file  = "./configure/PNseq_32000";

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char *input_file  = argv[1];
    const char *output_file = argv[2];

    char *src_data     = (char *)calloc(LDPC_N, sizeof(char));
    char *encoded_data = (char *)calloc(LDPC_N, sizeof(char));

    LDPCCodec *ldpc_encode = new LDPCCodec();
    ldpc_encode->init_encode(ldpc_pchk_file, enc_config_file);

    FILE *file = fopen(input_file, "rb");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    unsigned char byte;
    int bit_count = 0;
    while (fread(&byte, 1, 1, file) == 1 && bit_count + 8 <= LDPC_K) {
        for (int b = 7; b >= 0; --b) {
            src_data[bit_count++] = (byte >> b) & 1;
        }
    }
    fclose(file);

    while (bit_count < LDPC_K) {
        src_data[bit_count++] = 0;
    }

    ldpc_encode->encode(encoded_data, src_data);

    int *watermark = (int *)malloc(LDPC_N * sizeof(int));
    FILE *watermarkFP = fopen(watermark_file, "rb");
    if (!watermarkFP) {
        printf("Can't open the watermark file\n");
        return 1;
    }
    fread(watermark, sizeof(int), LDPC_N, watermarkFP);
    fclose(watermarkFP);

    FILE *genome_file_ptr = fopen(output_file, "w");
    if (!genome_file_ptr) {
        printf("Error opening file %s for writing.\n", output_file);
        return 1;
    }


    for (int j = 0; j < LDPC_N; j++) {
        int bit1 = encoded_data[j];
        int bit2 = watermark[j];
        char base;
        if (bit1 == 0 && bit2 == 0) base = 'A';
        else if (bit1 == 0 && bit2 == 1) base = 'T';
        else if (bit1 == 1 && bit2 == 0) base = 'G';
        else base = 'C';
        fputc(base, genome_file_ptr);
    }
    fclose(genome_file_ptr);

    free(src_data);
    free(encoded_data);
    free(watermark);

    return 0;
}
