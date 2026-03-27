#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "files_work.h"
#include "parser.h"

int getopt(int argc) {
  if (argc != 2) {
    print_err("FATAL", "you should run app with one parameter");
    return FATAL_RUNTIME;
  }
  return SUCCESS;
}

const char* tmp_filename = ".tmp";
const char* buf_filename = ".buf";

int main(int argc, char** argv) {
  if (getopt(argc) != 0) {
    return FATAL_RUNTIME;
  }
  char* src_filename = argv[1];

  if (copy_files(src_filename, tmp_filename) != 0) {
    print_err("FATAL", "files can't copied");
    return FATAL_RUNTIME;
  }

  comment_destroyer();
  // unite_lines();

  // dbg_print_file(tmp_filename);

  parse_line();

  // remove(tmp_filename);

  return SUCCESS;
}
