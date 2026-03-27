#ifndef FILES_WORK_H_
#define FILES_WORK_H_

#include <stdio.h>

#include "err.h"

int copy_files(const char* src_filename, const char* dst_filename);
int comment_destroyer();
int unite_lines();
int dbg_print_file(const char* filename);
#endif  // !FILES_WORK_H_
