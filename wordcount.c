#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#define READBUFSIZE 501

void processFile(FILE *fp, int *result);
void processLine(char *line, int *result);

int main(int argc, char** argv)
{
	FILE *fin;
	
	if (argc == 1) {
		printf("Usage: ./wordcount requires an input file.\n");
		return EXIT_FAILURE;
	}

	for (int i = 1; i < argc; i++) {
		if ((fin = fopen(argv[i], "r")) == NULL) {
			fprintf(stderr, "%s will not open. Skipping.\n", argv[i]);
			continue;
		}

		int result[] = {0, 0, 0};
		processFile(fin, result);
		printf("Total Lines = %d\n", result[0]);
		printf("Total Words = %d\n", result[1]);
		printf("Total Characters = %d\n", result[2]);
	}

	return EXIT_SUCCESS;
}

void processFile(FILE *fp, int *result)
{	
	char readbuf[READBUFSIZE];
	int linec = 0;
	while (fgets(readbuf, READBUFSIZE, fp) != NULL) {
		linec++;
		processLine(readbuf, result);
	}

	result[0] = linec;
	fclose(fp);
}

void processLine(char *c, int *result) {
	size_t characters = 0;
	size_t words = 0;
	
	// Skip white spaces
	while (*c && isspace(*c)) {
		characters++;
		c++;
	}

	// Count words
	while(*c) {
		words++;
		while(*c && !isspace(*c)) {
			characters++;
			c++;
		}
		
		while(*c && isspace(*c)) {
			characters++;
			c++;
		}
	}

	result[1] += words;
	result[2] += characters;
}
