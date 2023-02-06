// Copyright 2023 Eric Kim
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define READBUFSIZE 501

enum options { Lines, Words, Characters, All };

void parseOption(char **args, int argc, int *argindex, enum options *option);
void printResult(enum options option, int *result, char *filename);
void processFile(FILE *fp, int *result);
void processLine(char *line, int *result);

int main(int argc, char** argv) {
  enum options option;
  int fileindex;
  FILE *fin;
  int totallines = 0;
  parseOption(argv, argc, &fileindex, &option);

  for (; fileindex < argc; fileindex++) {
    if ((fin = fopen(argv[fileindex], "r")) == NULL) {
      fprintf(stderr, "%s will not open. Skipping.\n", argv[fileindex]);
    } else {
      int result[] = { 0, 0, 0};  // [lines, words, characters]
      processFile(fin, result);
      printResult(option, result, argv[fileindex]);
      totallines += result[0];
    }
  }

  if (option == All) {
    printf("Total Lines = %d\n", totallines);
  }

  return EXIT_SUCCESS;
}

// Given the program arguments, set the correct option and reposition the
// index to point at a file name. Exit program if no input file is
// found in the arguments.
void parseOption(char **args, int argc, int *argindex, enum options *option) {
  *argindex = 1, *option = All;

  if (argc > 1) {
    if (strncmp(args[*argindex], "-l", 2) == 0) {
      *option = Lines;
    } else if (strncmp(args[*argindex], "-w", 2) == 0) {
      *option = Words;
    } else if (strncmp(args[*argindex], "-c", 2) == 0) {
      *option = Characters;
    }

    while (*argindex < argc &&
          (strncmp(args[*argindex], "-l", 2) == 0 ||
           strncmp(args[*argindex], "-w", 2) == 0 ||
           strncmp(args[*argindex], "-c", 2) == 0)) {
      ++*argindex;
    }
  }

  if (argc == 1 || *argindex == argc) {
    fprintf(stderr, "Usage: ./wordcount requires an input file.\n");
    exit(EXIT_FAILURE);
  }
}

// Print the formatted count for a given file based on the option
// and the result array
void printResult(enum options option, int *result, char *filename) {
  if (option == All) {
      printf(" %d %d %d %s\n",
              result[Lines],
              result[Words],
              result[Characters],
              filename);
  } else {
      printf("%d\n", result[option]);
  }
}

// Process the file and store the line, word, and
// character count in the result array
void processFile(FILE *fp, int *result) {
  char readbuf[READBUFSIZE];
  int linec = 0;
  while (fgets(readbuf, READBUFSIZE, fp) != NULL) {
    linec++;
    processLine(readbuf, result);
  }

  result[0] = linec;
  fclose(fp);
}

// Process a string for number of words and characters
// and store the result in the result array
void processLine(char *c, int *result) {
  size_t characters = 0;
  size_t words = 0;
  while (*c && isspace(*c)) {
    characters++;
    c++;
  }

  while (*c) {
    words++;
    while (*c && !isspace(*c)) {
      characters++;
      c++;
    }

    while (*c && isspace(*c)) {
      characters++;
      c++;
    }
  }

  result[Words] += words;
  result[Characters] += characters;
}
