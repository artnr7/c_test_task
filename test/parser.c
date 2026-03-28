#include "../src/parser.h"

#include <check.h>
#include <string.h>

#include "../src/err.h"
#include "test.h"

#define STR_TEST_BUF_SIZE 32

START_TEST(entered_line_check_test) {
  char opts[][STR_TEST_BUF_SIZE] = {"use", "call", "fewfwe"};
  char lines[][STR_TEST_BUF_SIZE] = {"use", "use  fewfewf fewf we",
                                     "use fefew.so"};
  char exp_errs[] = {WARN_TOO_FEW_ARGUMENTS, WARN_TOO_MANY_ARGUMENTS, SUCCESS};

  for (int i = 0; i < 3; ++i) {
    char* str = strtok(lines[i], " ");
    (void)str;
    int err = entered_line_check(&(opts[i]), lines[i], i);
    ck_assert_int_eq(err, exp_errs[i]);
  }
}
END_TEST

Suite* parser(void) {
  Suite* s = suite_create("parser tests");
  TCase* tc_parser_core = tcase_create("parser core");

  tcase_add_test(tc_parser_core, entered_line_check_test);

  //
  suite_add_tcase(s, tc_parser_core);

  return s;
}
