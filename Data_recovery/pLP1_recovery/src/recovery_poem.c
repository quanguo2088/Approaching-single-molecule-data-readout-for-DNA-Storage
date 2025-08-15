#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    FILE *input_file = fopen("../results/information.txt", "r");
    if (!input_file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    size_t num_chars = 945 * 8;
    char *dec_results = (char *)malloc(num_chars);
    if (!dec_results) {
        perror("Memory allocation failed");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    if (fread(dec_results, sizeof(char), num_chars, input_file) != num_chars) {
        fprintf(stderr, "Error reading file or file is too short\n");
        free(dec_results);
        fclose(input_file);
        return EXIT_FAILURE;
    }
    fclose(input_file);

    char audio[945] = {0};
    for (int i = 0; i < 945; i++) {
        for (int j = 0; j < 8; j++) {
            audio[i] += (dec_results[8 * i + j] - '0') << (7 - j);
        }
    }

    FILE *output_file = fopen("../results/Poetry_of_recovery.txt", "wb");
    if (!output_file) {
        perror("Error opening output file");
        free(dec_results);
        return EXIT_FAILURE;
    }

    fwrite(audio, sizeof(char), 945, output_file);
    fclose(output_file);

    free(dec_results);
    return EXIT_SUCCESS;
}
