#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 300000
#define MAX_QUERY_NAME 200
#define MAX_SEQ_NAME 200

int main(int argc, char* argv[]) {
    // Check if the required arguments are passed
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <query_file> <seq_file>\n", argv[0]);
        return 1;
    }

    // Declare file pointers and file names
    FILE *queryFile = NULL;
    FILE *seqFile = NULL;
    char queryName[MAX_QUERY_NAME] = {0};
    char seqName[MAX_SEQ_NAME] = {0};

    // Read file names from arguments
    sscanf(argv[1], "%s", queryName);
    sscanf(argv[2], "%s", seqName);

    // Open query file for reading
    queryFile = fopen(queryName, "rt");
    if (queryFile == NULL) {
        perror("Cannot open query file");
        return 1;
    }

    // Open seq file for writing
    seqFile = fopen(seqName, "wt");
    if (seqFile == NULL) {
        perror("Cannot open seq file");
        fclose(queryFile);
        return 1;
    }

    // Buffers to hold lines from the query file
    char buff[MAXLEN] = {0};
    char buff1[MAXLEN] = {0};
    char buff2[MAXLEN] = {0};
    char buff3[MAXLEN] = {0};
    
    // Process the query file
    while (1) {
        // Read four lines at a time
        if (fgets(buff1, MAXLEN, queryFile) == NULL ||
            fgets(buff, MAXLEN, queryFile) == NULL ||
            fgets(buff2, MAXLEN, queryFile) == NULL ||
            fgets(buff3, MAXLEN, queryFile) == NULL) {
            break; // Exit loop if we reach the end of the file
        }

        // Get the length of the current sequence
        int seqLength = strlen(buff) - 1;  // Subtract 1 to ignore newline

        // Filter sequences based on length
        if (seqLength >= 30000 && seqLength <= 35000) {
        // if (seqLength >= 1) {
            // Write the valid sequence to the seq file
            fprintf(seqFile, "%s%s%s%s", buff1, buff, buff2, buff3);
        }
    }

    // Close the files
    fclose(queryFile);
    fclose(seqFile);

    return 0;
}
