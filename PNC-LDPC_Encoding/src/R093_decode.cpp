#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "ldpc_codec.h"

#define LDPC_K 29760
#define LDPC_N 32000

const char *enc_config_file = "./configure/R093.gen";
const char *ldpc_pchk_file  = "./configure/R093.pchk";

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <soft_inf_file> <output_file>\n", argv[0]);
        return 1;
    }


    const char *soft_inf_file = argv[1];
    const char *output_file = argv[2];

    int i = 0;

    LDPCCodec *ldpc_decode = new LDPCCodec();
    ldpc_decode->init_decode(ldpc_pchk_file, enc_config_file);


    double *lratio = (double *)calloc(LDPC_N, sizeof(double));
    char *rec_data = (char *)calloc(LDPC_K, sizeof(char));

    FILE *soft_inf_file_ptr = fopen(soft_inf_file, "r");
    if (soft_inf_file_ptr == NULL) {
        perror("Error opening soft_inf_file");
        return 1;
    }

    for (i = 0; i < LDPC_N; i++) {
        double p0, p1;
        if (fscanf(soft_inf_file_ptr, "%lf %lf", &p0, &p1) != 2) {
            fprintf(stderr, "Error reading line %d from %s\n", i + 1, soft_inf_file);
            fclose(soft_inf_file_ptr);
            return 1;
        }
        lratio[i] = p1 / p0;
    }

    fclose(soft_inf_file_ptr);

    //----LDPC(32000, 29760) decoding
    int ldpc_chk = ldpc_decode->LR_decode_soft(rec_data, lratio, 200);

    FILE *output_file_ptr = fopen(output_file, "w");
    if (output_file_ptr == NULL) {
        perror("Error opening output file");
        return 1;
    }

    for (i = 0; i < LDPC_K; i++) {
        fprintf(output_file_ptr, "%d", rec_data[i]);
    }

    fclose(output_file_ptr);

    free(lratio);
    free(rec_data);

    return 0;
}
