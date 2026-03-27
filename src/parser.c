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
    return WARN_TOO_MUCH_ARGUMENTS;
  }
  return SUCCESS;
}

int parse_use_line(const char* line, const int line_num, void** dll) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, line_num) != 0) {
    return 1;
  }

  if (*dll) {
    dlclose(*dll);
  }

  dlerror();  // flush
  *dll = dlopen(opt, RTLD_LAZY);
  if (!(*dll)) {
    print_err_with_line_num("FATAL", dlerror(), line_num, line);
    return 10;
  }

  return SUCCESS;
}

extern const char* tmp_filename;

int parse_call_line(const char* line, const int line_num, void** dll) {
  char* opt = NULL;
  if (entered_line_check(&opt, line, line_num) != 0) {
    return 1;
  }

  if (!(*dll)) {
    print_err_with_line_num("FATAL", "lib not loaded", line_num, line);
    return 10;
  }

  dlerror();  // flush

  void (*fn)(void);
  fn = dlsym(*dll, opt);
  const char* err = dlerror();
  if (err) {
    // print_err_with_line_num("FATAL", "function not found", line_num, line);
    print_err_with_line_num("FATAL", err, line_num, line);
    return 11;
  }
  fn();

  return 0;
}

const int STR_BUF_SIZE = 128;

int parse_line() {
  FILE* tmp_f = fopen(tmp_filename, "r");
  if (!tmp_filename) {
    print_err("FATAL", "file not found");
    return FATAL_FILE_NOT_FOUND;
  }

  int err_code = SUCCESS;
  int line_num = 1;
  char* entered_cmd = NULL;
  char* line_wout_slashn = malloc(STR_BUF_SIZE * sizeof(char));
  void* dll = NULL;

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, tmp_f) > 0 && err_code == SUCCESS) {
    memset(line_wout_slashn, '\0', STR_BUF_SIZE);  // обнуляем строку
    // Убираем из строки \n для удобной работы
    int len = strlen(line) - 1;
    if (line[len] != '\n') {
      ++len;
    }
    line_wout_slashn = strncpy(line_wout_slashn, line, len);
    entered_cmd = strtok(line_wout_slashn, " ");

    // Исходя из того какая команда была введена свитчимся в нужную функцию
    if (strcmp("use", entered_cmd) == 0) {
      err_code = parse_use_line(line, line_num, &dll);
    } else if (strcmp("call", entered_cmd) == 0) {
      err_code = parse_call_line(line, line_num, &dll);
    } else {
      print_err_with_line_num("WARN", "syntax error", line_num, line);
    }

    ++line_num;
  }
  fclose(tmp_f);

  if (dll) {
    dlclose(dll);
  }
  free(line_wout_slashn);
  if (line) {
    free(line);
  }

  return err_code;
}
