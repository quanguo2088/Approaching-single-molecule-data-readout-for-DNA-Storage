// #include <cstdlib>
// #include <cstdio>
// #include <cstring>
// #include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

#define N_TYPE 3 // '0', '1', 'E'
const char element[N_TYPE] = {'0', '1', 'E'};

int main(int argc, char *argv[])
{
    const int len_cw = 32000;

    double threshold_erasure = 0.0;
    char noisy_cw_file[500] = {0};
    char merged_cw_file[500] = {0};

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [noisy_cw_file] [merged_cw_file] [threshold]", argv[0]);
        return 1;
    }

    sscanf(argv[1], "%s", noisy_cw_file);
    sscanf(argv[2], "%s", merged_cw_file);
    sscanf(argv[3], "%lf", &threshold_erasure);


	FILE *fcurrent_error = NULL;
	char errname[500] = {0};
	sprintf(errname, "../results/bit_error_befor_decoding.txt");
	fcurrent_error = fopen(errname, "a");
	if (fcurrent_error == NULL)
		printf("Can not open %s\n", errname);



    FILE *fpr_noisy = NULL;
    if ((fpr_noisy = fopen(noisy_cw_file, "r")) == NULL)
    {
        fprintf(stderr, "Could not open file %s!\n", noisy_cw_file);
        return 1;
    }

    char *noisy_codewords = (char *)calloc(len_cw, sizeof(char));
    char *consensus_codewords = (char *)calloc(len_cw, sizeof(char)); // '0'/'1'/'E'
    unsigned *unique_pos[N_TYPE];
    for (int j = 0; j < N_TYPE; ++j)
        *(unique_pos + j) = (unsigned *)calloc(len_cw, sizeof(unsigned));


    unsigned count = 0;
    while (fscanf(fpr_noisy, "%s\n", noisy_codewords) != EOF)
    {
        ++count;
        for (int nb = 0; nb < len_cw; ++nb)
        {
            switch (*(noisy_codewords + nb))
            {
            case '0':
                ++*(*(unique_pos + 0) + nb);
                break;
            case '1':
                ++*(*(unique_pos + 1) + nb);
                break;
            case 'E':
                ++*(*(unique_pos + 2) + nb);
                break;
            default:
                break;
            }
        }
    }
    fclose(fpr_noisy);

    char ref_codeword[70000]={0};
    FILE *fref=NULL;
    fref=fopen("../data/original_codeword.txt","rt");
    fscanf(fref,"%s",ref_codeword);	
    fclose(fref);


    unsigned num_erasure = 0;
    double current_erasure_rate = 0;

    int tmp_val;
    int bin_fre[N_TYPE] = {0};
    int arr_sub[N_TYPE] = {0};

    //---- Bit-wise majority voting
    for (int sb = 0; sb < len_cw; ++sb)
    {
        if (*(*(unique_pos + 0) + sb) + *(*(unique_pos + 1) + sb) + *(*(unique_pos + 2) + sb) == 0)
        {
            *(consensus_codewords + sb) = 'E';
        }
        else
        {
            for (int j = 0; j < N_TYPE-1; ++j)
            {
                *(bin_fre + j) = *(*(unique_pos + j) + sb);
                *(arr_sub + j) = j;
            }
            for (int ti = 1; ti < N_TYPE-1; ++ti)
            {
                for (int tj = 0; tj < N_TYPE-1 - ti; ++tj)
                {
                    if (*(bin_fre + tj) < *(bin_fre + tj + 1))
                    {
                        tmp_val = *(bin_fre + tj);
                        *(bin_fre + tj) = *(bin_fre + tj + 1);
                        *(bin_fre + tj + 1) = tmp_val;

                        tmp_val = *(arr_sub + tj);
                        *(arr_sub + tj) = *(arr_sub + tj + 1);
                        *(arr_sub + tj + 1) = tmp_val;
                    }
                }
            }

            if (*(bin_fre + 0) > *(bin_fre + 1))
            {
                *(consensus_codewords + sb) = *(element + *(arr_sub + 0));
            }
            else if (*(bin_fre + 0) == *(bin_fre + 1))
            {
                *(consensus_codewords + sb) = 'E';
            }

        }
        if(*(consensus_codewords + sb) =='E')
                ++num_erasure;
    }

    current_erasure_rate = 1.0 * num_erasure / len_cw;

    int n_error = 0;
    int n_earse = 0;
    for (int i = 0; i < 22680; i++) 
    {
        if (consensus_codewords[i] != ref_codeword[i] && consensus_codewords[i] != 'E') 
        {
            n_error++;
        }
        else if (consensus_codewords[i] != ref_codeword[i] && consensus_codewords[i] == 'E')
        {
            n_earse++;
        }
    }

    double erro_rate = (n_error * 1.0) / (22680);
    double erasure_rate = (n_earse * 1.0) / (22680);
    printf("\nError rate: %.8lf, Erasure rate: %.8lf\n", erro_rate, erasure_rate);
    fprintf(fcurrent_error, "%d %d %lf %lf\n", n_error, n_earse, erro_rate, erasure_rate);
    fflush(fcurrent_error);	

    FILE *fpw_merged = NULL;
    if ((fpw_merged = fopen(merged_cw_file, "w")) == NULL)
    {
        fprintf(stderr, "Could not open file %s!\n", merged_cw_file);
        return 1;
    }
    fwrite(consensus_codewords, 1, len_cw, fpw_merged);
    fprintf(fpw_merged, "\n");
    fclose(fpw_merged);

    printf("%d codewords have been merged!\n\n", count);

	fclose(fcurrent_error);
    return 0;
}
