#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdint>
#include <cassert>

int main(int argc, char *argv[])
{
    double threshold_erasure = 0.0;
    sscanf(argv[1], "%lf", &threshold_erasure);

    char consensus_codewords[70000]={0};
    FILE *fpr_noisy = NULL;
    fpr_noisy=fopen("../results/consensus_codeword.txt","rt");
    fscanf(fpr_noisy,"%s",consensus_codewords);	
    fclose(fpr_noisy);

    char ref_codeword[70000]={0};
    FILE *fref=NULL;
    fref=fopen("../data/original_codeword.txt","rt");
    fscanf(fref,"%s",ref_codeword);	
    fclose(fref);

	// FILE *fcurrent_error = NULL;
	// char errname[500] = {0};
	// sprintf(errname, "../results/current_error.txt");
	// fcurrent_error = fopen(errname, "a");
	// if (fcurrent_error == NULL)
	// 	printf("Can not open %s\n", errname);


    int n_error = 0;
    int n_earse = 0;
    for (int i = 0; i < 32256*2; i++) 
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

    double erro_rate = (n_error * 1.0) / (32256*2);
    double erasure_rate = (n_earse * 1.0) / (32256*2);

    printf("Current error rate: %.8lf, erasure rate: %.8lf\n", erro_rate, erasure_rate);
    // fprintf(fcurrent_error, "%d %d %lf %lf\n", n_error, n_earse, erro_rate, erasure_rate);
    // fflush(fcurrent_error);	

    unsigned num_erasure = 0;
    for (size_t i = 0; i < 32256*2; i++)
    {
        if(consensus_codewords[i] == 'E')
            ++num_erasure;
    }
    double current_erasure_rate = (1.0 * num_erasure) / (32256*2);
    printf("Current erasure rate: %lf\n", current_erasure_rate);

    // printf("\nn_earse: %d, num_erasure: %d, erasure_rate: %lf, current_erasure_rate: %lf", 
    //     n_earse, num_erasure, erasure_rate, current_erasure_rate);

    if (current_erasure_rate < threshold_erasure)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}
