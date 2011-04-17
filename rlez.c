#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SEQ(file, byte, cnt)\
	do{	\
		fputc(0xff, file);\
		fputc(byte, file);\
		fputc(cnt, file);\
	}while(0)

int main(int argc, char* argv[]){
	int pre = -1;
	int chr;
	int cnt = 0;

	FILE* infile;
	FILE* outfile;
	char* outfilename;

	if (argc < 2){
		perror("usage: rlez filename\n");
		return -1;
	}

	outfilename = malloc(strlen(argv[1])+5);
	strcpy(outfilename, argv[1]);
	strcat(outfilename, ".rlez");
	infile = fopen(argv[1], "rb");
	outfile = fopen(outfilename,"wb");

	if (infile == NULL || outfile == NULL){
		perror("rlez: file could not be opened.\n");
		return -1;
	}

	chr = fgetc(infile);
	while(chr != EOF){
		if (chr == pre){
			if (cnt < 255)
				cnt++;
			else{
				SEQ(outfile, pre, cnt);
				cnt = 1;
			}
		}
		else{
			if (pre == 0xff){
				SEQ(outfile, pre, cnt);
			}
			else{
				if (cnt < 3){
					int i;
					for (i=0; i<cnt; i++){
						fputc(pre, outfile);
					}
				}
				else{
					SEQ(outfile, pre, cnt);
				}
			}
			pre = chr;
			cnt = 1;
		}
		chr = fgetc(infile);
	}
	SEQ(outfile, pre, cnt);

	fclose(infile);
	fclose(outfile);

	return 0;
}	
