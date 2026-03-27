#include "err.h"
void print_err(const char* err_code, const char* err) {
  fprintf(stderr, "%s: %s\n\n", err_code, err);
}
void print_err_with_line_num(const char* err_code, const char* err,
                             const int line_num, const char* line) {
  fprintf(stderr, "%s: %s: \n%d  %s\n\n", err_code, err, line_num, line);
}
