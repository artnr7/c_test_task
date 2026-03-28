#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
int entered_line_check(char** opt, const char* line, const int line_num);
int parse_use_line(const char* line, const int line_num, void** dll);
int manual_mode();
int auto_mode(const int argc, char** argv);

enum {
  NONE = 0,
  MANUAL,
  AUTO,
};

#endif
