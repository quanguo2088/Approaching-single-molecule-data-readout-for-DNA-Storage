#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>


#define MAXLEN 300000
char buff[MAXLEN] = {0};

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    int i, j;
    FILE *fp = NULL;
    fp = fopen(argv[1], "rt");
    if (fp == NULL) {
        perror("Error opening input file");
        return 1;
    }

    int sum = 0;
    FILE *fcheck = NULL;
    fcheck = fopen(argv[2], "wt");
    if (fcheck == NULL) {
        perror("Error opening output file");
        fclose(fp);
        return 1;
    }

    while (1) {
        fgets(buff, MAXLEN, fp);
        if (feof(fp))
            break;
        if (buff[0] == 'S' && buff[1] == 'i') {
            int kk = 0;
            for (i = 0; i < MAXLEN; i++) {
                if (buff[i + 43] == ',') {
                    break;
                }
                kk++;
            }

            if ((buff[43] - 48) == 0) {
                fprintf(fcheck, "1\n");
            } else {
                fprintf(fcheck, "0\n");
            }

            int num = 0;
            for (i = 0; i < kk; i++) {
                num += (buff[i + 43] - 48) * pow(10, (kk - 1 - i));
            }
            printf("Bit errors in the codeword after LDPC decoding: %d\n", num);

            sum += num;
        }
    }
    fclose(fp);
    fclose(fcheck);
    return 0;
}