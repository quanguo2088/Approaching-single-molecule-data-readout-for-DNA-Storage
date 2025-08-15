#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#define MAXLEN 300000

const double threshold = 1;

char buf[MAXLEN] = {0};
char buff[MAXLEN] = {0};
char buff1[MAXLEN] = {0};
char buff2[MAXLEN] = {0};
char buff3[MAXLEN] = {0};
char seq[MAXLEN] = {0};
char a[MAXLEN] = {0};
char base_synthetic[6][MAXLEN] = {0};
char buff_reverse[MAXLEN] = {0};
char base_synthetic_temp[MAXLEN] = {0};

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
	int i = 0, pos = 0, flag = 0, n = 0, m = 0, nn = 0, mm = 0;
	int start = 0, b = 0;
	char alignment[300000] = {0};
	int method = 0;
	char original_name[200];
	char fqname[200];
	char file_name[200];
	int cov;
	sscanf(argv[1], "%s", original_name);
	sscanf(argv[2], "%s", file_name);
	sscanf(argv[3], "%d", &m);
	sscanf(argv[4], "%d", &cov);
	sscanf(argv[5], "%s", fqname);

	char *read_buff = (char *)calloc(MAXLEN, 1);
	char *pattern_flag = (char *)calloc(MAXLEN, 1); // '0', '1, '2'
	char *polished_codeword = (char *)calloc(MAXLEN, 1);

	FILE *fp = NULL;
	FILE *findel = NULL;
	FILE *query = NULL;

	query = fopen(fqname, "rt");
	if (query == NULL)
	{
		printf("can not open the fq\n");
	}

	for (i = 0; i < m * cov; i++)
	{
		fgets(buff1, MAXLEN, query);
		fgets(buff, MAXLEN, query);
		fgets(buff2, MAXLEN, query);
		fgets(buff3, MAXLEN, query);
	}
	char resname[500] = {0};

	FILE *fpos = NULL;
	if ((fpos = fopen("../data/data_position.txt", "rt")) == NULL)
	{
		fprintf(stderr, "Could not open file ../data/data_position.txt!\n");
		return 1;
	}
	int start1, end1; 
	fscanf(fpos, "%d%d", &start1, &end1);
	fclose(fpos);

	int len_codeword = end1 - start1; 

	char *ref_PNseq = (char *)calloc(MAXLEN, 1);
	char *ref_seq = (char *)calloc(MAXLEN, 1);

	FILE *fpr_ref = NULL;
	if ((fpr_ref = fopen("../data/Plasmid_sequence.txt", "rt")) == NULL)
	{
		fprintf(stderr, "Could not open file ../data/Plasmid_sequence.txt!\n");
		return 1;
	}
	fgets(ref_seq, MAXLEN, fpr_ref);
	unsigned refer_len = strlen(ref_seq) - 1;
	for (unsigned l = 0; l < refer_len; l++) 
	{
		if (ref_seq[l] == 'A' || ref_seq[l] == 'T')
			ref_seq[l] = '0';
		else if (ref_seq[l] == 'G' || ref_seq[l] == 'C')
			ref_seq[l] = '1';
	}
	fclose(fpr_ref);

	FILE *fpr_PN = NULL;
	if ((fpr_PN = fopen(original_name, "rt")) == NULL)
	{
		fprintf(stderr, "Could not open file %s!\n", original_name);
		return 1;
	}
	fgets(ref_PNseq, MAXLEN, fpr_PN);
	fgets(ref_PNseq, MAXLEN, fpr_PN);
	fclose(fpr_PN);

	unsigned PN_len = strlen(ref_PNseq) - 1;

	fp = fopen(file_name, "rt");
	for (i = 0; i < 2; i++)
		fgets(buff, MAXLEN, fp);
	int k = 0;

	FILE *fpw_cw = NULL;
	sprintf(resname, "../results/polished_codeword.txt");
	if ((fpw_cw = fopen(resname, "a")) == NULL)
	{
		fprintf(stderr, "Could not open file %s!\n", resname);
		return 1;
	}

	int start_minimap = 0;
	unsigned n_error = 0;
	unsigned n_earse = 0;
	unsigned num_read = 0;

	while (1)
	{
		fscanf(fp, "%*s %d", &flag);
		if (feof(fp))
			break;

		++num_read;
		if (flag == 4 || flag > 16 || flag == 2048)
		{
			fgets(buff, MAXLEN, fp);
			pos = -1;
			if (flag == 4)
			{
				fgets(buff1, MAXLEN, query);
				fgets(buff, MAXLEN, query);
				fgets(buff2, MAXLEN, query);
				fgets(buff3, MAXLEN, query);
			}
			continue;
		}

		fscanf(fp, "%d %d %d %s %*s %*s %*s %s %[^\n]s", &pos, &start_minimap, &b, alignment, seq, a);

		memset(buff, '\0', sizeof(buff));
		fgets(buff1, MAXLEN, query);
		fgets(buff, MAXLEN, query);
		fgets(buff2, MAXLEN, query);
		fgets(buff3, MAXLEN, query);

		start_minimap = start_minimap - 1;
		if (flag == 16)
			flag = 1;

		int read_len = strlen(buff) - 1;
		if (flag == 1) 
		{
			memset(buff_reverse, '\0', sizeof(buff_reverse));
			for (i = 0; i < read_len; i++)
			{
				buff_reverse[i] = buff[i];
			}
			inv(buff_reverse, read_len);
			complement(buff_reverse, buff, read_len);
		}

		findel=fopen("temp.txt","wt");
		if(flag==0)
		{
			fprintf(findel,"%d %d\n",start_minimap,0);
			fprintf(findel,"%s",buff);
			
		}
		if(flag==1)
		{
			fprintf(findel,"%d %d\n",start_minimap,1);
			fprintf(findel,"%s",buff);
		}

		memset(buff1, '\0', sizeof(buff1));
		memset(buff2, '\0', sizeof(buff2));
		memset(buff3, '\0', sizeof(buff3));

		if(pos>=0)
		{
			int seq_len1=strlen(alignment); 
			int kk=0;
			int num=0;
			int ii=0;
			for(i=0,kk=0;i<seq_len1;i++,kk++)
			{
				if(alignment[i]=='S')
				{
					num=0;
					for(int j=0;j<kk;j++)
					{
						num+=((int)alignment[j+ii]-48)*pow(10,(kk-j-1));
					}
					ii+=kk+1;
					kk=-1;	
					for(int jj=0;jj<num;jj++)
					{
						fprintf(findel,"%d",1);
					}
				}
				if(alignment[i]=='M')
				{
					num=0;
					for(int j=0;j<kk;j++)
					{
						num+=((int)alignment[j+ii]-48)*pow(10,(kk-j-1));
					}
					ii+=kk+1;
					kk=-1;	
					for(int jj=0;jj<num;jj++)
					{
						fprintf(findel,"%d",0);
					}
				}
				
				if(alignment[i]=='I')
				{
					num=0;
					for(int j=0;j<kk;j++)
					{
						num+=((int)alignment[j+ii]-48)*pow(10,(kk-j-1));
					}
					ii+=kk+1;
					kk=-1;
					for(int jj=0;jj<num;jj++)
					{
						fprintf(findel,"%d",1);
					}
				}
				if(alignment[i]=='D')
				{
					num=0;
					for(int j=0;j<kk;j++)
					{
						num+=((int)alignment[j+ii]-48)*pow(10,(kk-j-1));
					}
					ii+=kk+1;
					kk=-1;
					for(int jj=0;jj<num;jj++)
					{
						fprintf(findel,"%d",2);
					}
				}
			}
			fprintf(findel,"\n");		
		}
		fclose(findel);
		memset(alignment,'\0',sizeof(alignment));

		findel=fopen("temp.txt","rt");
		memset(buff,'\0',sizeof(buff));
		memset(buff1,'\0',sizeof(buff1));
		memset(buff2,'\0',sizeof(buff2));

		fscanf(findel,"%d%d",&start,&flag);
		if(feof(findel))
		break;
		fscanf(findel,"%s",buff);
		fscanf(findel,"%s",alignment);
		int alignmentLength=strlen(alignment);

		int j=0;
		int seq_len=strlen(buff);
		for(i=0;i<alignmentLength;i++)
		{
			if(alignment[i]=='0' || alignment[i]=='1')
			{
				buff1[i]=buff[j];
				j++;
			}
			if(alignment[i]=='2')
			{
				buff1[i]='X';
			}
		}
		j=0;
		for(i=0;i<alignmentLength;i++)
		{
			if(alignment[i]!='1')
			{
				buff2[j]=buff1[i];
				j++;
			}
		}

		int polished_len = 0;
		polished_len = strlen(buff2) - 1;

		memset(polished_codeword, 'E', refer_len); 
		for (int ni = 0; ni < polished_len; ++ni) 
		{
			if (*(buff2 + ni) == 'A' || *(buff2 + ni) == 'T')
				*(polished_codeword + (start + ni) % refer_len) = '0';
			else if (*(buff2 + ni) == 'G' || *(buff2 + ni) == 'C')
				*(polished_codeword + (start + ni) % refer_len) = '1';
			else
				*(polished_codeword + (start + ni) % refer_len) = 'E';
		}

		fwrite((polished_codeword + start1), 1, len_codeword, fpw_cw);
		fprintf(fpw_cw, "\n");

		for (i = start1; i < end1; i++)
		{
			if (polished_codeword[i % refer_len] != ref_seq[i % refer_len] &&
				polished_codeword[i % refer_len] != 'E')
			{
				n_error++;
			}
			else if (polished_codeword[i % refer_len] != ref_seq[i % refer_len] &&
					polished_codeword[i % refer_len] == 'E')
			{
				n_earse++;
			}
		}
		fclose(findel);

	}
	fclose(fp);
	fclose(fpw_cw);
	return 0;
}



