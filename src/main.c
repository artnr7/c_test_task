#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "parser/parser.h"

bool getopt(int argc) {
  if (argc == 1 || argc >= 3) {
    printf("You should run app with one parameter\n");
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  (void)argv;

  if (!getopt(argc)) {
    return 1;
  }

  char* filename = NULL;
  if (argc >= 2) {
    filename = argv[1];
  }

  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    printf("File not found\n");
    return 1;
  }
  parse_line(f);

  fclose(f);
  return 0;
}
