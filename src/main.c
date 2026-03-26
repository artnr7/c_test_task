#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <dlfcn.h>

#include "parser/parser.h"

int getopt(int argc) {
  if (argc != 2) {
    printf("ERR: you should run app with one parameter\n");
    return 10;
  }
  return 0;
}

int main(int argc, char** argv) {
  if (getopt(argc) != 0) {
    return 10;
  }
  char* filename = argv[1];

  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    printf("ERR: file not found\n");
    return 11;
  }

  parse_line(f);

  fclose(f);
  return 0;
}
