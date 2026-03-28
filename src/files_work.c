#include "files_work.h"

#include <stdlib.h>
#include <string.h>

#include "err.h"

int copy_files(const char* src_filename, const char* dst_filename) {
  FILE* src_f = fopen(src_filename, "r");
  if (!src_f) {
    print_err("FATAL", "file not found");
    return FATAL_FILE_NOT_FOUND;
  }

  FILE* dst_f = fopen(dst_filename, "w");
  if (!dst_f) {
    fclose(src_f);
    print_err("FATAL", "file can't be created");
    return FATAL_FILE_NOT_FOUND;
  }

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, src_f) > 0) {
    fputs(line, dst_f);
  }
  free(line);

  fclose(src_f);
  fclose(dst_f);

  return SUCCESS;
}

int strip_comments_and_join_continuation_lines(const char* tmp_filename,
                                               const char* buf_filename) {
  FILE* tmp_f = fopen(tmp_filename, "r");
  if (!tmp_f) {
    print_err("FATAL", "file not found");
    return FATAL_FILE_NOT_FOUND;
  }

  FILE* buf = fopen(buf_filename, "w");
  if (!buf) {
    print_err("FATAL", "file can't be created");
    return FATAL_FILE_CANT_BE_CREATED;
  }

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, tmp_f) > 0) {
    // Пытаемся убрать комментарии
    char* sharp = strchr(line, '#');
    if (sharp) {
      *sharp = '\n';
      *(sharp + 1) = '\0';
    }
    // Пытаемся соединить строки
    char* bslash = strchr(line, '\\');
    if (bslash) {
      char* slashn = bslash + 1;
      if (slashn && *slashn == '\n') {
        *bslash = '\0';
      }
    }
    // Кладём получившуюся строку в буфер
    fputs(line, buf);
  }
  free(line);

  fclose(tmp_f);
  fclose(buf);

  remove(tmp_filename);
  rename(buf_filename, tmp_filename);

  return SUCCESS;
}

int dbg_print_file(const char* filename) {
  FILE* f = fopen(filename, "r");
  if (!f) {
    print_err("FATAL", "file not found");
    return FATAL_FILE_NOT_FOUND;
  }

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, f) > 0) {
    printf("%s", line);
  }
  free(line);

  fclose(f);

  return SUCCESS;
}
