#include "parser.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"

int entered_line_check(char** opt, const char* line, const int line_num) {
  *opt = strtok(NULL, " ");
  if (*opt == NULL) {  // здесь должен быть
    print_err_with_line_num("WARN", "too few options: ignored", line_num, line);
    return WARN_TOO_FEW_ARGUMENTS;
  }

  char* tok = strtok(NULL, " ");
  if (tok != NULL) {  // здесь не должен быть
    print_err_with_line_num("WARN", "too much options: ignored", line_num,
                            line);
    return WARN_TOO_MANY_ARGUMENTS;
  }
  return SUCCESS;
}

int parse_use_line(const char* line, const int line_num, void** dll) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, line_num) != SUCCESS) {
    return WARN_SYNTAX;
  }

  if (*dll) {
    dlclose(*dll);
  }

  dlerror();  // flush
  *dll = dlopen(opt, RTLD_LAZY);
  if (!(*dll)) {
    print_err_with_line_num("FATAL", dlerror(), line_num, line);
    return FATAL_SO_NOT_LOADED;
  }

  return SUCCESS;
}

int parse_call_line(const char* line, const int line_num, void** dll) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, line_num) != 0) {
    return WARN_SYNTAX;
  }

  if (!(*dll)) {
    print_err_with_line_num("FATAL", "lib not loaded", line_num, line);
    return FATAL_SO_NOT_LOADED;
  }

  dlerror();  // flush

  void (*fn)(void);
  fn = dlsym(*dll, opt);
  const char* err = dlerror();
  if (err) {
    print_err_with_line_num("FATAL", "function not found", line_num, line);
    return FATAL_FUNCTION_NOT_FOUND;
  }
  fn();

  return SUCCESS;
}

extern const char* tmp_filename;

enum {
  STR_BUF_SIZE = 256,
};

int process_line(const char* line, char* line_wout_slashn, int* line_num,
                 void** dll) {
  int err = SUCCESS;

  memset(line_wout_slashn, '\0', STR_BUF_SIZE);  // обнуляем строку
  // Убираем из строки \n для удобной работы
  int len = strlen(line) - 1;
  if (line[len] != '\n') {
    ++len;
  }
  line_wout_slashn = strncpy(line_wout_slashn, line, len);
  char* entered_cmd = strtok(line_wout_slashn, " ");

  // Исходя из того какая команда была введена свитчимся в нужную функцию
  if (strcmp("use", entered_cmd) == 0) {
    err = parse_use_line(line, *line_num, dll);
  } else if (strcmp("call", entered_cmd) == 0) {
    err = parse_call_line(line, *line_num, dll);
  } else {
    print_err_with_line_num("WARN", "syntax error", *line_num, line);
    err = WARN_SYNTAX;
  }

  ++(*line_num);

  return err;
}

int manual_mode() {
  int err = SUCCESS;

  char line[STR_BUF_SIZE] = {'\0'};
  char line_wout_slashn[STR_BUF_SIZE] = {'\0'};
  int line_num = 1;

  void* dll = NULL;

  while (err < FATAL && fgets(line, sizeof(line), stdin)) {
    err = process_line(line, line_wout_slashn, &line_num, &dll);
  }

  if (dll) {
    dlclose(dll);
  }

  return err;
}

int auto_mode() {
  FILE* tmp_f = fopen(tmp_filename, "r");
  if (!tmp_filename) {
    print_err("FATAL", "file not found");
    return FATAL_FILE_NOT_FOUND;
  }
  int err = SUCCESS;

  char* line = NULL;
  char line_wout_slashn[STR_BUF_SIZE] = {'\0'};
  int line_num = 1;

  void* dll = NULL;

  size_t n = 0;
  while (err < FATAL && getline(&line, &n, tmp_f) > 0) {
    err = process_line(line, line_wout_slashn, &line_num, &dll);
  }
  fclose(tmp_f);

  if (dll) {
    dlclose(dll);
  }
  if (line) {
    free(line);
  }

  return err;
}

int parse_line(const int mode) {
  int err = SUCCESS;

  switch (mode) {
    case MANUAL:
      err = manual_mode();
      break;
    case AUTO:
      err = auto_mode();
      break;
  }

  return err;
}
