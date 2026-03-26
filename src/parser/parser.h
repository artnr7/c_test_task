#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

void parse_line(FILE* f);

void parse_call_line(const char* line, const int lines_cnt);

#endif
