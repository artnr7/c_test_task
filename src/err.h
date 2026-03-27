#ifndef ERR_H_
#define ERR_H_

#include "stdio.h"

void print_err(const char* err_code, const char* err);
void print_err_with_line_num(const char* err_code, const char* err,
                             const int line_num, const char* line);

enum {
  SUCCESS = 0,
  // 100 - 199 WARN
  WARN = 100,
  WARN_RUNTIME,
  WARN_FILE_NOT_FOUND,
  WARN_FILE_CANT_BE_CREATED,

  WARN_TOO_FEW_ARGUMENTS,
  WARN_TOO_MANY_ARGUMENTS,

  WARN_SYNTAX,
  // DLL
  WARN_SO_NOT_LOADED,
  WARN_FUNCTION_NOT_FOUND,

  // 200 - 299 FATAL
  FATAL = 200,
  FATAL_RUNTIME,
  FATAL_FILE_NOT_FOUND,
  FATAL_FILE_CANT_BE_CREATED,

  FATAL_TOO_FEW_ARGUMENTS,
  FATAL_TOO_MANY_ARGUMENTS,

  FATAL_SYNTAX,

  // DLL
  FATAL_SO_NOT_LOADED,
  FATAL_FUNCTION_NOT_FOUND,
};

#endif
