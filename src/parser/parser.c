#include "parser.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

void print_line_check_warn(const char* err, const char* line, const int n) {
  fprintf(stderr, "WARN: %s: ignored:\n%d  %s\n\n", err, n, line);
}

int entered_line_check(char** opt, const char* line, const int lines_cnt) {
  *opt = strtok(NULL, " ");
  if (*opt == NULL) {  // здесь должен быть
    print_line_check_warn("too few options", line, lines_cnt);
    return 1;
  }

  char* tok = strtok(NULL, " ");
  if (tok != NULL) {  // здесь не должен быть
    print_line_check_warn("too much options", line, lines_cnt);
    return 2;
  }
  return 0;
}

int parse_use_line(const char* line, const int lines_cnt, void** dll) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, lines_cnt) != 0) {
    return 1;
  }

  if (*dll) {
    dlclose(*dll);
  }

  // printf("opt = %s\n", opt);
  dlerror();  // flush
  *dll = dlopen(opt, RTLD_LAZY);
  if (!(*dll)) {
    fprintf(stderr, "ERR: %s\n", dlerror());
    return 10;
  }

  return 0;
}

int parse_call_line(const char* line, const int lines_cnt, void** dll) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, lines_cnt) != 0) {
    return 1;
  }
  if (!(*dll)) {
    fprintf(stderr, "ERR: lib not loaded\n");
    return 10;
  }

  dlerror();  // flush

  void (*fn)(void);
  fn = dlsym(*dll, opt);
  const char* err = dlerror();
  if (err) {
    fprintf(stderr, "ERR: function not found: %s\n", err);
    return 11;
  }
  fn();

  return 0;
}

#define STR_BUF_SIZE 128

int parse_line(FILE* f) {
  int err_code = 0;
  char* line = NULL;
  size_t n = 0;

  int lines_cnt = 1;
  char* entered_cmd = NULL;
  char* line_wout_slashn = malloc(STR_BUF_SIZE * sizeof(char));
  void* dll = NULL;

  while (getline(&line, &n, f) > 0 && err_code < 10) {
    memset(line_wout_slashn, '\0', STR_BUF_SIZE);
    // printf("lines_cnt = %d\n----------\n", lines_cnt);
    // printf("line = %s\n", line);
    int len = strlen(line) - 1;
    if (line[len] != '\n') {
      ++len;
    }

    line_wout_slashn = strncpy(line_wout_slashn, line, len);
    entered_cmd = strtok(line_wout_slashn, " ");

    if (strcmp("use", entered_cmd) == 0) {
      err_code = parse_use_line(line, lines_cnt, &dll);
    } else if (strcmp("call", entered_cmd) == 0) {
      err_code = parse_call_line(line, lines_cnt, &dll);
    } else {
      printf("Syntax Error");
    }
    // printf("err = %d\n", err_code);

    ++lines_cnt;
  }

  if (dll) {
    dlclose(dll);
  }
  free(line_wout_slashn);
  free(line);

  return err_code;
}
