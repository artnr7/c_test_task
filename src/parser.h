#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

int manual_mode();
int auto_mode(const int argc, char** argv);

enum {
  NONE = 0,
  MANUAL,
  AUTO,
};

#endif
