#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    char *dec_results; 
    size_t num_chars = 4032 * 8;
    FILE *file;

    dec_results = (char *)malloc(num_chars * sizeof(char));
    if (dec_results == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char audio[4032] = {0};

    // information bit
    file = fopen("../results/information.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        free(dec_results); 
        return 1;
    }

    size_t result = fread(dec_results, sizeof(char), num_chars, file);
    if (result != num_chars) {
        fprintf(stderr, "Error reading file or file is too short\n");
        free(dec_results); 
        fclose(file);
        return 1;
    }

    int t1 = 0; 
    for (int i = 0; i < 4032; i++) 
    {
        for (int j = 8 * i; j < 8 * (i + 1); j++) 
        {
            audio[t1] += (dec_results[j] - '0') * (1 << (7 - (j % 8)));
        }
        t1++;
    }

    // Recovered file
    FILE *fpoutput1 = fopen("../results/poem.txt", "wb");
    if (fpoutput1 == NULL) {
        fprintf(stderr, "Error opening output file\n");
        free(dec_results);
        return 1;
    }
    fwrite(audio, sizeof(char), 4032, fpoutput1);
    fclose(fpoutput1); 

    free(dec_results);
    fclose(file);

    return 0;
}