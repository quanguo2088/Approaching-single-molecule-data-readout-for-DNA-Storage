
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 3000000

char buf[300000]={0}; 
char buff[300000]={0};
char buff1[300000]={0};
char buff2[300000]={0};
char buff3[300000]={0};
char base_reverse[300000]={0};

int main(int argc ,char* argv[])
{
	int len;
	int i=0,j=0,k=0,kk=0;
	
	char reads[300000]={0};
	for(i=0;i<300000;i++)
	{
		reads[i]=0;
	}
	
	FILE *original=NULL;
	char oriname[200]={0};
	sscanf(argv[1],"%s",oriname);

	original=fopen(oriname,"rt");
	
	FILE *findel=NULL;
	findel=fopen("../data/pn_sequence.fa","wt");
	while(1)
	{
		fgets(buff,MAXLEN,original);
		if(feof(original))
		break;
		kk=strlen(buff)-1;
		for(i=0;i<kk;i++)
		{
			if(buff[i]=='A')
			{
				reads[i]='A';
			}
			else if(buff[i]=='T')
			{
				reads[i]='T';
			}
			else if(buff[i]=='G')
			{
				reads[i]='A';
			}
			else if(buff[i]=='C')
			{
				reads[i]='T';
			}
		}
		fprintf(findel,">%d\n",j);
		for(i=0;i<1;i++)
		fprintf(findel,"%s",reads);
		fprintf(findel,"%s\n",reads);
		j++;
		memset(buff,'\0',sizeof(buff));
		memset(reads,'\0',sizeof(reads));
	}

	fclose(original);
	fclose(findel);
	
	return 0;
}
