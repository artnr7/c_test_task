#include <check.h>

#include "test.h"

Suite* parser(void) {
  Suite* s = suite_create("parser tests");
  TCase* tc_parse_line = tcase_create("parse_line");

  // tcase_add_test(tc_parse_line, );
  //
  suite_add_tcase(s, tc_parse_line);

  return s;
}
