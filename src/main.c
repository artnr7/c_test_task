#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

// ERRORS DESC
// one-digit err is WARN
// two-digit err is FATAL

void print_err(const char* err_code, const char* err) {
  fprintf(stderr, "%s: %s\n\n", err_code, err);
}

int getopt(int argc) {
  if (argc != 2) {
    print_err("ERR", "you should run app with one parameter");
    return 10;
  }
  return 0;
}

int comment_destroyer(FILE* f, FILE* tmp_f) {
  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, f) > 0) {
    char* sharp = strchr(line, '#');
    if (sharp) {
      printf("%c", *sharp);
      *sharp = '\n';
      *(sharp + 1) = '\0';
    }
    fputs(line, tmp_f);
  }

  return 0;
}

void dbg_print_file(FILE* f) {
  rewind(f);
  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, f) > 0) {
    printf("%s", line);
  }
}

int main(int argc, char** argv) {
  if (getopt(argc) != 0) {
    return 10;
  }
  char* filename = argv[1];

  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    print_err("FATAL", "file not found");
    return 11;
  }

  FILE* tmp_f = fopen("tmp", "w");
  if (!tmp_f) {
    fclose(f);
    print_err("FATAL", "file can't be created");
    return 12;
  }

  comment_destroyer(f, tmp_f);
  fclose(f);
  fclose(tmp_f);

  tmp_f = fopen("tmp", "r");
  if (!tmp_f) {
    print_err("FATAL", "file not found");
    return 13;
  }

  // dbg_print_file(tmp_f);

  parse_line(tmp_f);

  remove("tmp");

  return 0;
}
