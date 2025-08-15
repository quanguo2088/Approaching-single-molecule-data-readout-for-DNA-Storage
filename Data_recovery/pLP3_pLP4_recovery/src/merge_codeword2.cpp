#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <ctime>

#define N_TYPE 2 // '0', '1'
const char element[N_TYPE] = {'0', '1'};
const int len_cw = 32256;


void save_counts(const char *file, unsigned counts[][N_TYPE]) {
    FILE *fp = fopen(file, "w");
    if (fp != NULL) {
        for (int i = 0; i < len_cw; ++i) {
            fprintf(fp, "%u %u\n", counts[i][0], counts[i][1]);
        }
        fclose(fp);
    } else {
        fprintf(stderr, "Could not write to file %s!\n", file);
    }
}


void load_counts_text(const char *file, unsigned counts[][N_TYPE]) {
    FILE *fp = fopen(file, "r");
    if (fp != NULL) 
    {
        int idx=0;
        while (fscanf(fp, "%u %u", &counts[idx][0], &counts[idx][1]) == 2) 
        {
            idx++;
        }
        fclose(fp);
        
    } else 
    {
        memset(counts, 0, len_cw * N_TYPE * sizeof(unsigned));
    }
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s [noisy_cw_file] [merged_cw_file] [fqindex] [threshold]\n", argv[0]);
        return 1;
    }

    char noisy_cw_file[500] = {0}, merged_cw_file[500] = {0}, counts_file[500] = {0}, counts_file2[500] = {0};
    double threshold_erasure = 0.0;
    int fqindex = 0;

    sscanf(argv[1], "%s", noisy_cw_file);
    sscanf(argv[2], "%s", merged_cw_file);
    sscanf(argv[3], "%d", &fqindex);
    sscanf(argv[4], "%lf", &threshold_erasure);

    // FILE *fcurrent_error = NULL;
    // char errname[500] = {0};
    // sprintf(errname, "../results/current_error2.txt");
    // fcurrent_error = fopen(errname, "a");
    // if (fcurrent_error == NULL)
    //     printf("Can not open %s\n", errname);

    // char ref_codeword[70000] = {0};
    // FILE *fref = fopen("../reference/original_codeword2.txt", "rt");
    // if (fref == NULL) {
    //     fprintf(stderr, "Could not open reference codeword file!\n");
    //     return 1;
    // }
    // fscanf(fref, "%s", ref_codeword);	
    // fclose(fref);

    FILE *fpr_noisy = fopen(noisy_cw_file, "r");
    if (fpr_noisy == NULL) {
        fprintf(stderr, "Could not open file %s!\n", noisy_cw_file);
        return 1;
    }

    unsigned counts[len_cw][N_TYPE] = {0};
    
    sprintf(counts_file, "../results/488_count_%d.txt", fqindex-1);
    sprintf(counts_file2, "../results/488_count_%d.txt", fqindex);

    load_counts_text(counts_file, counts); // 读取历史统计记录

    char noisy_codewords[len_cw + 1] = {0};
    while (fscanf(fpr_noisy, "%s", noisy_codewords) != EOF) {
        for (int nb = 0; nb < len_cw; ++nb) {
            switch (noisy_codewords[nb]) {
                case '0': counts[nb][0]++; break;
                case '1': counts[nb][1]++; break;
                // 忽略 'E'
                default: break;
            }
        }
    }
    fclose(fpr_noisy);

    save_counts(counts_file2, counts); // 保存统计记录并生成历史文件

    unsigned num_erasure = 0;
    char consensus_codewords[len_cw] = {0};

    //---- 简化的 Bit-wise majority voting
    for (int sb = 0; sb < len_cw; ++sb)
    {
        unsigned count0 = counts[sb][0];
        unsigned count1 = counts[sb][1];
        
        if (count0 == count1) {
            consensus_codewords[sb] = 'E';
        }
        else if (count0 > count1) {
            consensus_codewords[sb] = '0';
        }
        else {
            consensus_codewords[sb] = '1';
        }
        
        if(consensus_codewords[sb] == 'E')
            ++num_erasure;
    }

    // int n_error = 0;
    // int n_erasure = 0;
    // for (int i = 0; i < len_cw; i++) 
    // {
    //     if (consensus_codewords[i] != ref_codeword[i] && consensus_codewords[i] != 'E') 
    //     {
    //         n_error++;
    //     }
    //     else if (consensus_codewords[i] != ref_codeword[i] && consensus_codewords[i] == 'E')
    //     {
    //         n_erasure++;
    //     }
    // }

    // double error_rate = (n_error * 1.0) / len_cw;
    // double erasure_rate = (n_erasure * 1.0) / len_cw;
    // printf("Current error rate: %.8lf, erasure rate: %.8lf\n", error_rate, erasure_rate);
    // if (fcurrent_error != NULL) {
    //     fprintf(fcurrent_error, "%d %d %lf %lf\n", n_error, n_erasure, error_rate, erasure_rate);
    //     fflush(fcurrent_error);	
    //     fclose(fcurrent_error);
    // }

    double current_erasure_rate = 1.0 * num_erasure / len_cw;
    printf("Current erasure rate (pLP4): %lf\n", current_erasure_rate);

    if(current_erasure_rate > threshold_erasure)
    {
        return 1;
    }

    FILE *fpw_merged = fopen(merged_cw_file, "w");
    if (fpw_merged == NULL) {
        fprintf(stderr, "Could not open file %s!\n", merged_cw_file);
        return 1;
    }
    fwrite(consensus_codewords, 1, len_cw, fpw_merged);
    fprintf(fpw_merged, "\n");
    fclose(fpw_merged);

    return 0;
}
