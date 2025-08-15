#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>


#define MAXLEN 300000

const double threshold = 0.06;

char buff[MAXLEN] = {0};
char buff1[MAXLEN] = {0};
char buff2[MAXLEN] = {0};
char buff3[MAXLEN] = {0};
char seq[MAXLEN] = {0};
char a[MAXLEN] = {0};
char buff_reverse[MAXLEN] = {0};
char alignment[MAXLEN] = {0};
char result_array[MAXLEN] = {0}; 

void inv(char *x, int n)
{
	int i, j, m = (n - 1) / 2;
	char temp;
	for (i = 0; i <= m; i++)
	{
		j = n - 1 - i;
		temp = x[i];
		x[i] = x[j];
		x[j] = temp;
	}
	return;
}

void complement(char *input, char *output, int len)
{
	int i;
	static char temp[MAXLEN] = {0};
	for (i = 0; i < len; i++)
	{
		temp[i] = input[i];
		if (temp[i] == 'A')
		{
			output[i] = 'T';
		}
		else if (temp[i] == 'T')
		{
			output[i] = 'A';
		}
		else if (temp[i] == 'G')
		{
			output[i] = 'C';
		}
		else if (temp[i] == 'C')
		{
			output[i] = 'G';
		}
		else
			output[i] = 'N';
	}
}

int main(int argc, char *argv[])
{
	int i = 0, j = 0, pos = 0, flag = 0, b = 0;
	int start = 0, end = 0;
	char original_name[200];
	char file_name[200];
	char fqname[200];
	int cov, m = 0;
	int ins = 0, sub = 0, del = 0;
	int ki = 0;
	int polished_len = 0;
	double indel_rate = 0;

	sscanf(argv[1], "%s", original_name); 
	sscanf(argv[2], "%s", file_name); //sam
	sscanf(argv[3], "%d", &m);
	sscanf(argv[4], "%d", &cov);
	sscanf(argv[5], "%s", fqname); //fastq

	char *read_buff = (char *)calloc(MAXLEN, 1);
	char *polished_codeword1 = (char *)calloc(MAXLEN, 1);
	char *polished_codeword2 = (char *)calloc(MAXLEN, 1);


	// seq.fastq
	FILE *query = NULL;
	query = fopen(fqname, "rt");
	if (query == NULL)
	{
		printf("can not open the %s\n", fqname);
	}

	for (i = 0; i < m * cov; i++)
	{
		fgets(buff1, MAXLEN, query);
		fgets(buff, MAXLEN, query);
		fgets(buff2, MAXLEN, query);
		fgets(buff3, MAXLEN, query);
	}

	int start3=0, end3=32256, start4=0, end4=32256;

	int len_codeword1 = 32256; // file 3
	int len_codeword2 = 32256; // file 3

	// unsigned refer_len3 = 43342;
	// unsigned refer_len4 = 43335;

	char *ref_PNseq3 = (char *)calloc(MAXLEN, 1);
	char *ref_PNseq4 = (char *)calloc(MAXLEN, 1);

	char *ref_seq3 = (char *)calloc(MAXLEN, 1);
	char *ref_seq4 = (char *)calloc(MAXLEN, 1);

	FILE *fpr_ref = NULL;
	if ((fpr_ref = fopen("../data/plasmid_sequence.txt", "rt")) == NULL)
	{
		fprintf(stderr, "Could not open file ../data/plasmid_sequence.txt!\n");
		return 1;
	}
	fgets(ref_seq3, MAXLEN, fpr_ref); // 487
	fgets(ref_seq4, MAXLEN, fpr_ref); // 488

	// 487
	unsigned refer_len3 = strlen(ref_seq3) - 1;
	// printf("Length of No.1 PN sequence: %u\n", refer_len3);
	for (unsigned l = 0; l < refer_len3; l++)
	{
		if (ref_seq3[l] == 'A' || ref_seq3[l] == 'T')
			ref_seq3[l] = '0';
		else if (ref_seq3[l] == 'G' || ref_seq3[l] == 'C')
			ref_seq3[l] = '1';
	}

	// 488
	unsigned refer_len4 = strlen(ref_seq4) - 1;
	// printf("Length of No.2 PN sequence: %u\n", refer_len4);
	for (unsigned l = 0; l < refer_len4; l++)
	{
		if (ref_seq4[l] == 'A' || ref_seq4[l] == 'T')
			ref_seq4[l] = '0';
		else if (ref_seq4[l] == 'G' || ref_seq4[l] == 'C')
			ref_seq4[l] = '1';
	}
	fclose(fpr_ref);


	FILE *fpr_PN = NULL;
	if ((fpr_PN = fopen(original_name, "rt")) == NULL)
	{
		fprintf(stderr, "Could not open file %s!\n", original_name);
		return 1;
	}
	fgets(ref_PNseq3, MAXLEN, fpr_PN);
	fgets(ref_PNseq3, MAXLEN, fpr_PN);
	fgets(ref_PNseq4, MAXLEN, fpr_PN);
	fgets(ref_PNseq4, MAXLEN, fpr_PN);
	fclose(fpr_PN);

	unsigned PN_len3 = strlen(ref_PNseq3) - 1;
	unsigned PN_len4 = strlen(ref_PNseq4) - 1;
	// printf("PN_len3: %d\n", PN_len3);

	// sam文件
	FILE *fp = NULL;
	fp = fopen(file_name, "rt");
	for (i = 0; i < 3; i++)
		fgets(buff, MAXLEN, fp);

	// FILE *ferror = NULL;
	// char errname[500] = {0};
	// sprintf(errname, "../results/bit_error_befor_decoding.txt");
	// ferror = fopen(errname, "a");
	// if (ferror == NULL)
	// 	printf("Can not open bit_error_befor_decoding.txt\n");

	FILE *fpw_cw1 = NULL;
	char resname[500] = {0};
	sprintf(resname, "../results/polished_codeword1.txt");
	if ((fpw_cw1 = fopen(resname, "w")) == NULL) // 追加写入
	{
		fprintf(stderr, "Could not open file %s!\n", resname);
		return 1;
	}

	FILE *fpw_cw2 = NULL;
	char resname2[500] = {0};
	sprintf(resname2, "../results/polished_codeword2.txt");
	if ((fpw_cw2 = fopen(resname2, "w")) == NULL) // 追加写入
	{
		fprintf(stderr, "Could not open file %s!\n", resname2);
		return 1;
	}

	// FILE *fpw_cw3 = NULL;
	// char resname3[500] = {0};
	// sprintf(resname3, "../results/polished_codeword.txt");
	// if ((fpw_cw3 = fopen(resname3, "a")) == NULL) // 追加写入
	// {
	// 	fprintf(stderr, "Could not open file %s!\n", resname3);
	// 	return 1;
	// }

	int start_minimap = 0;
	int minED;
	unsigned n_error = 0;
	unsigned n_earse = 0;
	unsigned num_read = 0;
	unsigned num_aligned = 0;
	char samid[MAXLEN] = {0};
	char fqid[MAXLEN] = {0};
	memset(samid, '\0', sizeof(samid));
	memset(fqid, '\0', sizeof(fqid));

	while (1)
	{
		fscanf(fp, "%s %d", samid, &flag);
		if (feof(fp))
			break;

		++num_read;
		if (flag == 4 || flag > 16)
		{
			fgets(buff, MAXLEN, fp);
			pos = -1;
			if (flag == 4)
			{
				if ((num_aligned + 1) % 10000 == 0)
				{
					printf("finish %d\n", num_aligned);
				}
				fgets(buff, MAXLEN, query);
				fgets(buff, MAXLEN, query);
				fgets(buff, MAXLEN, query);
				fgets(buff, MAXLEN, query);
			}
			continue;
		}

		fscanf(fp, "%d %d %d %s %*s %*s %*s %s %[^\n]s", &pos, &start_minimap, &b, alignment, seq, a);

		start = start_minimap - 1;

		fgets(fqid, MAXLEN, query);
		fgets(read_buff, MAXLEN, query);
		fgets(buff2, MAXLEN, query);
		fgets(buff3, MAXLEN, query);
		fqid[37] = '\0'; //id

		// 打印fqid和samid
		// printf("fqid: %s\n", fqid);
		// printf("samid: %s\n", samid);

		// 检查fqid与samid是否一致
		// for (size_t i = 0; i < 36; i++)
		// {
		// 	if (fqid[i+1] != samid[i])
		// 	{
		// 		printf("\n\nfqid: %s\n", fqid);
		// 		printf("samid: %s\n", samid);
		// 		printf("fqid and samid are not the same!\n");
		// 		break;
		// 	}
		// }

		if (strlen(seq) < 100)
		{
			continue;
		}

		memset(buff1, '\0', sizeof(buff1));
		memset(buff2, '\0', sizeof(buff2));
		// memset(buff3, '\0', sizeof(buff3));

		int read_len = strlen(read_buff) - 1;

		if (flag == 16)
		{
			memset(buff_reverse, '\0', sizeof(buff_reverse));
			for (i = 0; i < read_len; i++)
			{
				buff_reverse[i] = read_buff[i];
			}
			inv(buff_reverse, read_len);
			complement(buff_reverse, read_buff, read_len);
		}

		int alignlen = strlen(alignment);
		// printf("alignlen: %d\n", alignlen);

		int kk = 0;
		int num = 0;
		int ii = 0;
		int result_index = 0; 
		ins=0;
		del=0;
		// 487
		if (pos==0) 
		{
			// 计算indel位置
			for (i = 0, kk = 0; i < alignlen; i++, kk++) {
				if (alignment[i] == 'S' || alignment[i] == 'M' || alignment[i] == 'I' || alignment[i] == 'D') 
				{
					num = 0;
					for (int j = 0; j < kk; j++) 
					{
						num += ((int)alignment[j + ii] - 48) * pow(10, (kk - j - 1));
					}
					ii += kk + 1;
					kk = -1;

					char value = (alignment[i] == 'S' || alignment[i] == 'I') ? '1'
							: (alignment[i] == 'M') ? '0'
							: '2';

					for (int jj = 0; jj < num; jj++) 
					{
						result_array[result_index++] = value;
					}
				}
			}
			memset(alignment, '\0', sizeof(alignment));

			int alignlen2 = strlen(result_array);
			// printf("alignlen2: %d\n", alignlen2);

			// 修正碱基
			j=0;
			for(i=0;i<alignlen2;i++)
			{
				if(result_array[i]=='0' || result_array[i]=='1')
				{
					buff1[i]=read_buff[j];
					j++;
				}
				if(result_array[i]=='2')
				{
					buff1[i]='X';
				}
			}
			j=0;
			for(i=0;i<alignlen2;i++)
			{
				if(result_array[i]!='1')
				{
					buff2[j]=buff1[i];
					j++;
				}
			}
			polished_len = strlen(buff2) - 1;
			// printf("polished_len: %d\n", polished_len);
			// printf("start: %d\n", start);

			// 统计indel数量
			for (int i = 0; i < alignlen2; i++) {
				if (result_array[i] == '1') {
					ins++;
				} else if (result_array[i] == '2') {
					del++;
				}
			}
			indel_rate = (ins+del)*1.0 / read_len;
			// printf("indels: %d\n", ins+del);
			// printf("indel_rate: %lf\n", indel_rate);

			if (indel_rate > threshold)
			{
				continue;
			}
			
			num_aligned++;
			// 取上层码字序列
			memset(polished_codeword1, 'E', refer_len3); 
			for (int ni = 0; ni < polished_len; ++ni) 
			{
				if (*(buff2 + ni) == 'A' || *(buff2 + ni) == 'T')
					*(polished_codeword1 + (start + ni) % refer_len3) = '0';
				else if (*(buff2 + ni) == 'G' || *(buff2 + ni) == 'C')
					*(polished_codeword1 + (start + ni) % refer_len3) = '1';
				else
					*(polished_codeword1 + (start + ni) % refer_len3) = 'E';
			}
			fwrite(polished_codeword1, 1, len_codeword1, fpw_cw1);
			fprintf(fpw_cw1, "\n");

			// fwrite(polished_codeword1, 1, len_codeword1, fpw_cw3);
			// for (int ni = 0; ni < 32256; ++ni)
			// {
			// 	fprintf(fpw_cw3, "E");
			// }
			// fprintf(fpw_cw3, "\n");


			// n_error = 0;
			// n_earse = 0;

			// for (i = start; i < start+polished_len; i++)
			// {
			// 	if (polished_codeword1[i % refer_len3] != ref_seq3[i % refer_len3] &&
			// 		polished_codeword1[i % refer_len3] != 'E')
			// 	{
			// 		n_error++;
			// 	}
			// 	else if (polished_codeword1[i % refer_len3] != ref_seq3[i % refer_len3] &&
			// 			polished_codeword1[i % refer_len3] == 'E')
			// 	{
			// 		n_error++;
			// 		n_earse++;
			// 	}
			// }

			// double erro_rate = (n_error * 1.0) / (polished_len);
			// double erasure_rate = (n_earse * 1.0) / (polished_len);
			// printf("Bit error: %d (del. = %d)\n", n_error, n_earse);
			// printf("Bit error rate: %.8lf (del. = %.8lf)\n", erro_rate, erasure_rate);			

		}


		// 488
		if (pos==1) 
		{
			// 计算indel位置
			for (i = 0, kk = 0; i < alignlen; i++, kk++) {
				if (alignment[i] == 'S' || alignment[i] == 'M' || alignment[i] == 'I' || alignment[i] == 'D') 
				{
					num = 0;
					for (int j = 0; j < kk; j++) 
					{
						num += ((int)alignment[j + ii] - 48) * pow(10, (kk - j - 1));
					}
					ii += kk + 1;
					kk = -1;

					char value = (alignment[i] == 'S' || alignment[i] == 'I') ? '1'
							: (alignment[i] == 'M') ? '0'
							: '2';

					for (int jj = 0; jj < num; jj++) 
					{
						result_array[result_index++] = value;
					}
				}
			}
			memset(alignment, '\0', sizeof(alignment));

			int alignlen2 = strlen(result_array);
			// printf("alignlen2: %d\n", alignlen2);

			// 修正碱基
			j=0;
			for(i=0;i<alignlen2;i++)
			{
				if(result_array[i]=='0' || result_array[i]=='1')
				{
					buff1[i]=read_buff[j];
					j++;
				}
				if(result_array[i]=='2')
				{
					buff1[i]='X';
				}
			}
			j=0;
			for(i=0;i<alignlen2;i++)
			{
				if(result_array[i]!='1')
				{
					buff2[j]=buff1[i];
					j++;
				}
			}
			polished_len = strlen(buff2) - 1;
			// printf("polished_len: %d\n", polished_len);
			// printf("start: %d\n", start);
			
			// 统计indel数量
			for (int i = 0; i < alignlen2; i++) {
				if (result_array[i] == '1') {
					ins++;
				} else if (result_array[i] == '2') {
					del++;
				}
			}
			indel_rate = (ins+del)*1.0 / read_len;
			// printf("indels: %d\n", ins+del);
			// printf("indel_rate: %lf\n", indel_rate);
			if (indel_rate >= threshold)
			{
				continue;
			}
			
			num_aligned++;
			// 取上层码字序列
			memset(polished_codeword2, 'E', refer_len4); 
			for (int ni = 0; ni < polished_len; ++ni) 
			{
				if (*(buff2 + ni) == 'A' || *(buff2 + ni) == 'T')
					*(polished_codeword2 + (start + ni) % refer_len4) = '0';
				else if (*(buff2 + ni) == 'G' || *(buff2 + ni) == 'C')
					*(polished_codeword2 + (start + ni) % refer_len4) = '1';
				else
					*(polished_codeword2 + (start + ni) % refer_len4) = 'E';
			}
			fwrite(polished_codeword2, 1, len_codeword2, fpw_cw2);
			fprintf(fpw_cw2, "\n");

			// for (int ni = 0; ni < 32256; ++ni)
			// {
			// 	fprintf(fpw_cw3, "E");
			// }
			// fwrite(polished_codeword2, 1, len_codeword2, fpw_cw3);
			// fprintf(fpw_cw3, "\n");

			// n_error = 0;
			// n_earse = 0;
			// for (i = start; i < start+polished_len; i++)
			// {
			// 	if (polished_codeword2[i % refer_len4] != ref_seq4[i % refer_len4] &&
			// 		polished_codeword2[i % refer_len4] != 'E')
			// 	{
			// 		n_error++;
			// 	}
			// 	else if (polished_codeword2[i % refer_len4] != ref_seq4[i % refer_len4] &&
			// 			polished_codeword2[i % refer_len4] == 'E')
			// 	{
			// 		n_error++;
			// 		n_earse++;
			// 	}
			// }

			// double erro_rate = (n_error * 1.0) / (polished_len);
			// double erasure_rate = (n_earse * 1.0) / (polished_len);
			// printf("Bit error: %d (del. = %d)\n", n_error, n_earse);
			// printf("Bit error rate: %.8lf (del. = %.8lf)\n", erro_rate, erasure_rate);

		}
	}

	printf("Total processed reads in SAM: %u\n", num_read);
	printf("Aligned reads number: %d\n", num_aligned);

	fclose(fp);
	fclose(fpw_cw1);
	fclose(fpw_cw2);
	// fclose(ferror);

	return 0;
}
