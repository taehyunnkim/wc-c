// Copyright 2023 Eric Kim

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define READBUFSIZE 501

enum options { All, Words, Characters, Lines };

enum options parseOption(char *argument);
void printResult(enum options option, int *result, char *filename);
void processFile(FILE *fp, int *result);
void processLine(char *line, int *result);

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("Usage: ./wordcount requires an input file.\n");
    return EXIT_FAILURE;
  }

  FILE *fin;
  enum options option = parseOption(argv[1]);
  int index = 1;
  int totalLines = 0;

  if (option != All) {
    index = 2;
  }

  for (; index < argc; index++) {
    if ((fin = fopen(argv[index], "r")) == NULL) {
      fprintf(stderr, "%s will not open. Skipping.\n", argv[index]);
      continue;
    }

    int result[] = {0, 0, 0};
    processFile(fin, result);

    totalLines += result[0];
    printResult(option, result, argv[index]);
  }

  if (option == All) {
    printf("Total Lines = %d\n", totalLines);
  }

  return EXIT_SUCCESS;
}

enum options parseOption(char *argument) {
  if (strncmp(argument, "-l", 2) == 0) {
    return Lines;
  } else if (strncmp(argument, "-w", 2) == 0) {
    return Words;
  } else if (strncmp(argument, "-c", 2) == 0) {
    return Characters;
  } else {
    return All;
  }
}

void printResult(enum options option, int *result, char *filename) {
  switch (option) {
    case All:
      printf("%d %d %d %s\n", result[0], result[1], result[2], filename);
      break;
    case Lines:
      printf("%d\n", result[0]);
      break;
    case Words:
      printf("%d\n", result[1]);
      break;
    case Characters:
      printf("%d\n", result[2]);
      break;
  }
}

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

void processLine(char *c, int *result) {
  size_t characters = 0;
  size_t words = 0;

  // Skip white spaces
  while (*c && isspace(*c)) {
    characters++;
    c++;
  }

  // Count words
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

  result[1] += words;
  result[2] += characters;
}
