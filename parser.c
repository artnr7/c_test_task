#include "parser.h"

#include <stdlib.h>
#include <string.h>

void print_use_line_warn(const char* err, const char* line, const int n) {
  printf("WARN: %s: ignored:\n%d  %s\n\n", err, n, line);
}

void parse_use_line(const char* line, const int n, char* cmd) {
  if (strcmp("use", cmd) != 0) {
    return;
  }

  char* opt = strtok(NULL, " ");
  if (opt == NULL) {
    print_use_line_warn("too few options", line, n);
    return;
  }
  char* leks = strtok(NULL, " ");
  if (leks != NULL) {
    print_use_line_warn("too much options", line, n);
    return;
  }
}
void parse_call_line(const char* line, const int n, char* cmd) {
  if (strcmp("call", cmd) != 0) {
    return;
  }

  char* opt = strtok(NULL, " ");
  if (opt == NULL) {
    print_use_line_warn("too few options", line, n);
    return;
  }
  char* leks = strtok(NULL, " ");
  if (leks != NULL) {
    print_use_line_warn("too much options", line, n);
    return;
  }
}

void parseline(FILE* f) {
  char* line = NULL;
  size_t n = 100;

  int line_cnt = 1;
  char* leks = NULL;
  while (1) {
    int cnt = getline(&line, &n, f);
    if (cnt < 1) {
      break;
    }

    char* str = calloc(50, sizeof(char));
    // printf("%ld\n", strlen(line));
    int cp = strlen(line) - 1;
    if (line[cp] != '\n') {
      ++cp;
    }

    str = strncpy(str, line, cp);
    // printf("%s\n", str);

    leks = strtok(str, " ");
    // printf("%s", leks);

    parse_use_line(line, line_cnt, leks);

    str = strncpy(str, line, cp);
    // printf("%s\n", str);

    leks = strtok(str, " ");

    // printf("%s", leks);
    parse_call_line(line, line_cnt, leks);

    // printf("flag = %d, n = %ld\n", cnt, n);
    ++line_cnt;
    free(str);
  }

  free(line);
}
