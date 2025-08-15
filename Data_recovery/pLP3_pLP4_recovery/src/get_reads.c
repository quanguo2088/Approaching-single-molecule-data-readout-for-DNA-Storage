#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 3000000

// Function to process the DNA sequence and write it to the output file
void process_sequence(FILE *output_file, char *header, char *sequence, char *plus, char *quality) {
    char reads[300000] = {0};  // Reset the reads buffer
    int len = strlen(sequence) - 1;  // Ignore the newline at the end

    // Transform the sequence to the reverse complement
    for (int i = 0; i < len; i++) {
        if (sequence[i] == 'A') {
            reads[i] = 'A';
        } else if (sequence[i] == 'T') {
            reads[i] = 'T';
        } else if (sequence[i] == 'G') {
            reads[i] = 'A';  // Non-standard: G -> A (check if this is intended)
        } else if (sequence[i] == 'C') {
            reads[i] = 'T';  // Non-standard: C -> T (check if this is intended)
        }
    }

    // Write the processed sequence to the output file
    fprintf(output_file, "%s", header);
    fprintf(output_file, "%s\n", reads);
    fprintf(output_file, "%s", plus);
    fprintf(output_file, "%s", quality);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <m> <cov> <fq>\n", argv[0]);
        return EXIT_FAILURE;
    }

	char fq[200];

    // Parse input arguments
    int m = 0, cov = 0;

    sscanf(argv[1], "%d", &m);
    sscanf(argv[2], "%d", &cov);
    sscanf(argv[3], "%s", fq);

    // Open the query file (input FASTQ file)
    FILE *query_file = fopen(fq, "rt");
    if (query_file == NULL) {
        perror("Error opening query file");
        return EXIT_FAILURE;
    }

    // Open the output file (seq.fastq)
    FILE *output_file = fopen("../results/seq.fastq", "wt");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(query_file);  // Ensure we close the query file before exiting
        return EXIT_FAILURE;
    }

    // Temporary buffers for reading the FASTQ file
    char header[300000] = {0}, sequence[300000] = {0}, plus[300000] = {0}, quality[300000] = {0};

    // Skip the first m * cov sequences
    for (int i = 0; i < m * cov; i++) {
        fgets(header, MAXLEN, query_file);
        fgets(sequence, MAXLEN, query_file);
        fgets(plus, MAXLEN, query_file);
        fgets(quality, MAXLEN, query_file);
    }

    // Process and write the sequences to the output file
    int count = 0;
    while (count < cov && fgets(header, MAXLEN, query_file) != NULL) {
        fgets(sequence, MAXLEN, query_file);
        fgets(plus, MAXLEN, query_file);
        fgets(quality, MAXLEN, query_file);

        // Process the sequence and write to the output file
        process_sequence(output_file, header, sequence, plus, quality);
        count++;
    }

    // Close the files
    fclose(query_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}
