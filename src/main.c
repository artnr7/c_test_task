#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>

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
  if (getopt(argc) != SUCCESS) {
    return FATAL_RUNTIME;
  }
  char* src_filename = argv[1];

  if (copy_files(src_filename, tmp_filename) != SUCCESS) {
    print_err("FATAL", "files can't copied");
    return FATAL_RUNTIME;
  }

  strip_comments_and_join_continuation_lines();

  parse_line(MANUAL);
  // manual_mode();

  remove(tmp_filename);

  return SUCCESS;
}
