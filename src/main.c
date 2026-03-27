#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

int comment_destroyer(FILE* tmp_f) {
  rewind(tmp_f);
  FILE* buf = fopen("buf", "w");
  if (!buf) {
    print_err("FATAL", "file can't be created");
    return 10;
  }

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, tmp_f) > 0) {
    char* sharp = strchr(line, '#');
    if (sharp) {
      // printf("%c", *sharp);
      *sharp = '\n';
      *(sharp + 1) = '\0';
    }
    fputs(line, buf);
  }
  free(line);

  remove("tmp");
  rename("buf", "tmp");

  return 0;
}

int unite_lines(FILE* tmp_f) {
  rewind(tmp_f);
  FILE* buf = fopen("buf", "w");
  if (!buf) {
    print_err("FATAL", "file can't be created");
    return 10;
  }

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, tmp_f) > 0) {
    char* bslash = strchr(line, '\\');
    if (bslash) {
      char* slashn = bslash + 1;
      if (slashn && *slashn == '\n') {
        // printf("-----------%s", line);
        *bslash = '\0';
        // printf("=============%s", line);
      }
    }
    fputs(line, buf);
  }
  free(line);

  fclose(buf);

  remove("tmp");
  rename("buf", "tmp");

  return 0;
}

int copy_files(FILE* src, FILE* dst) {
  if (!tmp_f) {
    fclose(src_f);
    print_err("FATAL", "file can't be created");
    return 12;
  }

  if (!src || !dst) {
    print_err("FATAL", "file(s) not found");
    return 10;
  }

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, src) > 0) {
    // printf("%s", line);
    fputs(line, dst);
  }
  free(line);

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
  char* src_filename = argv[1];

  FILE* src_f = fopen(src_filename, "r");
  if (!src_f) {
    print_err("FATAL", "file not found");
    return 11;
  }

  const char* tmp_filename = "tmp";
  FILE* tmp_f = NULL;

  if (copy_files(src_f, tmp_f) != 0) {
    print_err("FATAL", "files can't copied");
    return 10;
  }

  fclose(tmp_f);
  tmp_f = fopen(tmp_filename, "r");
  if (!tmp_f) {
    print_err("FATAL", "file not found");
    return 13;
  }

  dbg_print_file(tmp_f);

  comment_destroyer(tmp_f);  // TODO: копию файла
  unite_lines(tmp_f);
  fclose(src_f);

  // parse_line(tmp_f);

  // remove(tmp_filename);

  return 0;
}
