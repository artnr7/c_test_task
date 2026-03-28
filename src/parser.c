#include "parser.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "files_work.h"

int entered_line_check(char** opt, const char* line, const int line_num) {
  *opt = strtok(NULL, " ");
  if (!(*opt)) {  // здесь должен быть
    print_err_with_line_num("WARN", "too few options: ignored", line_num, line);
    return WARN_TOO_FEW_ARGUMENTS;
  }

  char* tok = strtok(NULL, " ");
  if (tok) {  // здесь не должен быть
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

  void (*fn)(void) = dlsym(*dll, opt);
  const char* err = dlerror();
  if (err) {
    print_err_with_line_num("FATAL", "function not found", line_num, line);
    return FATAL_FUNCTION_NOT_FOUND;
  }
  fn();

  return SUCCESS;
}

#define STR_BUF_SIZE 256

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
  if (!entered_cmd) {
    print_err_with_line_num("WARN", "syntax error", *line_num, line);
    err = WARN_SYNTAX;
  }

  // Исходя из того какая команда была введена свитчимся в нужную функцию
  if (err == SUCCESS) {
    if (strcmp("use", entered_cmd) == 0) {
      err = parse_use_line(line, *line_num, dll);
    } else if (strcmp("call", entered_cmd) == 0) {
      err = parse_call_line(line, *line_num, dll);
    } else {
      print_err_with_line_num("WARN", "syntax error", *line_num, line);
      err = WARN_SYNTAX;
    }
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

int getopt(int argc) {
  if (argc != 2) {
    print_err("FATAL", "you should run app with one parameter");
    return FATAL_RUNTIME;
  }
  return SUCCESS;
}

int auto_mode(const int argc, char** argv) {
  if (getopt(argc) != SUCCESS) {
    return FATAL_RUNTIME;
  }
  const char* src_filename = argv[1];

  const char* tmp_filename = ".tmp";
  const char* buf_filename = ".buf";

  if (copy_files(src_filename, tmp_filename) != SUCCESS) {
    print_err("FATAL", "files can't copied");
    return FATAL_RUNTIME;
  }

  if (strip_comments_and_join_continuation_lines(tmp_filename, buf_filename) !=
      SUCCESS) {
    print_err("FATAL", "cant process source script file");
    return FATAL_RUNTIME;
  }

  FILE* tmp_f = NULL;
  if (open_file_for_reading(&tmp_f, tmp_filename) != SUCCESS) {
    return FATAL_FILE_CANT_BE_OPENED;
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
  remove(tmp_filename);

  return err;
}
