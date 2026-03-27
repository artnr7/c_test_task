#ifndef ERR_H_
#define ERR_H_

#include "stdio.h"

void print_err(const char* err_code, const char* err);
void print_err_with_line_num(const char* err_code, const char* err,
                             const int line_num, const char* line);

enum {
  SUCCESS = 0,
  // 100 - 199 WARN
  WARN_RUNTIME = 100,
  WARN_FILE_NOT_FOUND,
  WARN_FILE_CANT_BE_CREATED,

  WARN_TOO_FEW_ARGUMENTS,
  WARN_TOO_MUCH_ARGUMENTS,

  // 200 - 299 FATAL
  FATAL_RUNTIME = 200,
  FATAL_FILE_NOT_FOUND,
  FATAL_FILE_CANT_BE_CREATED,

  FATAL_TOO_FEW_ARGUMENTS,
  FATAL_TOO_MUCH_ARGUMENTS,
};

#endif
