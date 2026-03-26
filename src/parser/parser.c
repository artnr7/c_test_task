#include "parser.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

void print_line_check_warn(const char* err, const char* line, const int n) {
  printf("WARN: %s: ignored:\n%d  %s\n\n", err, n, line);
}

int entered_line_check(char** opt, const char* line, const int lines_cnt) {
  *opt = strtok(NULL, " ");
  if (*opt == NULL) {  // здесь должен быть
    print_line_check_warn("too few options", line, lines_cnt);
    return 1;
  }

  char* leks = strtok(NULL, " ");
  if (leks != NULL) {  // здесь не должен быть
    print_line_check_warn("too much options", line, lines_cnt);
    return 2;
  }
  return 0;
}

int parse_use_line(const char* line, const int lines_cnt) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, lines_cnt) != 0) {
    return 1;
  }

  // printf("opt = %s\n", opt);
  void* dll = dlopen(opt, RTLD_LAZY);
  if (!dll) {
    fprintf(stderr, "%s\n", dlerror());
    dlclose(dll);
    return 10;
  }
  dlerror();
  void (*example)(void);

  example = dlsym(dll, "example");

  example();

  dlclose(dll);
  return 0;
}

int parse_call_line(const char* line, const int lines_cnt) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, lines_cnt) != 0) {
    return 1;
  }

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

  while (getline(&line, &n, f) > 0 && !err_code) {
    memset(line_wout_slashn, '\0', STR_BUF_SIZE);
    printf("lines_cnt = %d\n----------\n", lines_cnt);
    // printf("line = %s\n", line);
    int len = strlen(line) - 1;
    if (line[len] != '\n') {
      ++len;
    }

    line_wout_slashn = strncpy(line_wout_slashn, line, len);
    entered_cmd = strtok(line_wout_slashn, " ");

    int err = 0;
    if (strcmp("use", entered_cmd) == 0) {
      // TODO: (artnr7) сделать обработку ошибок
      err = parse_use_line(line, lines_cnt);
    } else if (strcmp("call", entered_cmd) == 0) {
      err = parse_call_line(line, lines_cnt);
    } else {
      printf("Syntax Error");
    }

    if (err >= 10) {
      err_code = 10;
    }

    ++lines_cnt;
  }

  free(line_wout_slashn);
  free(line);
  return err_code;
}
