#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_path  = argv[1];
    const char *output_path = argv[2];

    FILE *input_file = fopen(input_path, "r");
    if (!input_file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    size_t num_chars = 3720 * 8;
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

    char audio[3720] = {0};
    for (int i = 0; i < 3720; i++) {
        for (int j = 0; j < 8; j++) {
            audio[i] += (dec_results[8 * i + j] - '0') << (7 - j);
        }
    }

    FILE *output_file = fopen(output_path, "wb");
    if (!output_file) {
        perror("Error opening output file");
        free(dec_results);
        return EXIT_FAILURE;
    }

    fwrite(audio, sizeof(char), 3716, output_file);
    fclose(output_file);

    free(dec_results);
    return EXIT_SUCCESS;
}
