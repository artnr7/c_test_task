#include "parser.h"

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

  char* dot = strchr(opt, '.');

  if (dot == NULL) {
    printf("ERR: wrong file format: corrupted:\n%d  %s\n\n", lines_cnt, opt);
    return 10;
  }

  if (!(*(dot + 1) == 's' && *(dot + 2) == 'o')) {
    printf("ERR: wrong file format: corrupted:\n%d  %s\n\n", lines_cnt, opt);
    return 11;
  }

  FILE* f = fopen(opt, "r");
  if (f == NULL) {
    printf("ERR: file not found: corrupted:\n%d  %s\n\n", lines_cnt, opt);
    return 12;
  }

  return 0;
}

int parse_call_line(const char* line, const int lines_cnt) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, lines_cnt) != 0) {
    return 1;
  }

  
  return 0;
}

int parse_line(FILE* f) {
  int err_code = 0;
  char* line = NULL;
  size_t n = 100;

  int lines_cnt = 1;
  char* entered_cmd = NULL;
  char* line_wout_slashn = calloc(50, sizeof(char));

  while (getline(&line, &n, f) > 0 && !err_code) {
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
