#include "files_work.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"

int open_two_files_for_cp(FILE** src, const char* src_filename, FILE** dst,
                          const char* dst_filename) {
  *src = fopen(src_filename, "r");
  if (!(*src)) {
    print_err("FATAL", "file not found");
    return FATAL_FILE_NOT_FOUND;
  }

  *dst = fopen(dst_filename, "w");
  if (!(*dst)) {
    fclose(*src);
    print_err("FATAL", "file can't be created");
    return FATAL_FILE_CANT_BE_CREATED;
  }
  return SUCCESS;
}

int open_file_for_reading(FILE** f, const char* filename) {
  *f = fopen(filename, "r");
  if (!(*f)) {
    print_err("FATAL", "file not found");
    return FATAL_FILE_NOT_FOUND;
  }
  return SUCCESS;
}

int copy_files(const char* src_filename, const char* dst_filename) {
  FILE* src_f = NULL;
  FILE* dst_f = NULL;
  if (open_two_files_for_cp(&src_f, src_filename, &dst_f, dst_filename) !=
      SUCCESS) {
    return FATAL_FILES_CANT_BE_COPIED;
  }

  char* line = NULL;
  size_t n = 0;
  while (getline(&line, &n, src_f) > 0) {
    fputs(line, dst_f);
  }
  if (line) {
    free(line);
  }

  fclose(src_f);
  fclose(dst_f);

  return SUCCESS;
}

int strip_comments_and_join_continuation_lines(const char* tmp_filename,
                                               const char* buf_filename) {
  FILE* tmp_f = NULL;
  FILE* buf = NULL;
  if (open_two_files_for_cp(&tmp_f, tmp_filename, &buf, buf_filename) !=
      SUCCESS) {
    return FATAL_FILES_CANT_BE_COPIED;
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
  FILE* f = NULL;
  if (open_file_for_reading(&f, filename) != SUCCESS) {
    return FATAL_FILE_CANT_BE_OPENED;
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
