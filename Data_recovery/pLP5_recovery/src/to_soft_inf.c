#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    FILE *fp_in = fopen(input_filename, "r");
    FILE *fp_out = fopen(output_filename, "w");

    if (!fp_in || !fp_out) {
        printf("Error opening input or output file.\n");
        return 1;
    }

    char ch;
    int count = 0;
    while ((ch = fgetc(fp_in)) != EOF && count < 32000) {
        if (ch == '0') {
            fprintf(fp_out, "0.9999 0.0001\n");
            count++;
        } else if (ch == '1') {
            fprintf(fp_out, "0.0001 0.9999\n");
            count++;
        } else if (ch == 'E') {
            fprintf(fp_out, "0.5 0.5\n");
            count++;
        } else if (ch == '\n' || ch == '\r') {
            continue;  // skip newline
        } else {
            printf("Invalid character at position %d: '%c'\n", count, ch);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
    }
    

    if (count < 32000) {
        printf("Warning: only %d bits found, expected 32000.\n", count);
    } else {
        printf("Success: converted 32000 bits to probability format.\n");
    }

    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
