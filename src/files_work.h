#ifndef FILES_WORK_H_
#define FILES_WORK_H_

#include <stdio.h>

int open_two_files_for_cp(FILE** src, const char* src_filename, FILE** dst,
                          const char* dst_filename);
int copy_files(const char* src_filename, const char* dst_filename);
int strip_comments_and_join_continuation_lines(const char* tmp_filename,
                                               const char* buf_filename);
int dbg_print_file(const char* filename);
#endif
