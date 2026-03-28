#include "../src/parser.h"

#include <check.h>
#include <dlfcn.h>
#include <string.h>

#include "../src/err.h"
#include "test.h"

#define STR_TEST_BUF_SIZE 32

START_TEST(entered_line_check_test) {
  char* opts[STR_TEST_BUF_SIZE] = {"use", "call", "fewfwe"};
  char lines[][STR_TEST_BUF_SIZE] = {"use", "use  fewfewf fewf we",
                                     "use fefew.so"};
  int exp_errs[] = {WARN_TOO_FEW_ARGUMENTS, WARN_TOO_MANY_ARGUMENTS, SUCCESS};

  for (int i = 0; i < 3; ++i) {
    char* str = strtok(lines[i], " ");
    (void)str;
    int err = entered_line_check(&(opts[i]), lines[i], i);
    ck_assert_int_eq(err, exp_errs[i]);
  }
}
END_TEST

START_TEST(parse_use_line_test) {
  char lines[][STR_TEST_BUF_SIZE] = {"use", "use fwe312312 f32f23",
                                     "use fgwefw3mock.so", "use ./mock.so"};
  void* dll[4] = {NULL};
  int exp_errs[] = {WARN_SYNTAX, WARN_SYNTAX, FATAL_SO_NOT_LOADED, SUCCESS};

  for (int i = 0; i < 4; ++i) {
    char* cmd = strtok(lines[i], " ");
    (void)cmd;
    int err = parse_use_line(lines[i], i, &(dll[i]));
    ck_assert_int_eq(err, exp_errs[i]);
  }
}

START_TEST(parse_call_line_test) {
  char lines[][STR_TEST_BUF_SIZE] = {"call", "call fwe312312 f32f23",
                                     "call rfg3fewwe_fn", "call ex"};
  void* dll[4] = {NULL};
  int exp_errs[] = {WARN_SYNTAX, WARN_SYNTAX, FATAL_SO_NOT_LOADED, SUCCESS};

  for (int i = 0; i < 4; ++i) {
    if (i == 3) {
      dll[3] = dlopen("./mock.so", RTLD_LAZY);
    }
    char* cmd = strtok(lines[i], " ");
    (void)cmd;
    int err = parse_call_line(lines[i], i, &(dll[i]));
    ck_assert_int_eq(err, exp_errs[i]);
    if (i == 3 && dll[3]) {
      dlclose(dll[3]);
    }
  }
}

START_TEST(process_line_test) {
  char* lines[] = {"call few", "use fewfw", "use", "call use", "fewf"};
  char line_wout_slashn[STR_BUF_SIZE] = {'\0'};
  void* dll = NULL;
  int exp_errs[] = {FATAL_SO_NOT_LOADED, FATAL_SO_NOT_LOADED, WARN_SYNTAX,
                    FATAL_SO_NOT_LOADED, WARN_SYNTAX};
  int line_num = 1;
  for (int i = 0; i < 5; ++i) {
    int err = process_line(lines[i], line_wout_slashn, &line_num, &dll, MANUAL);
    ck_assert_int_eq(err, exp_errs[i]);
  }
}

START_TEST(getopt_test) {
  for (int i = -2; i < 3; ++i) {
    if (i == 2) continue;

    int err = getopt(i);
    ck_assert_int_eq(err, FATAL_RUNTIME);
  }
  int err = getopt(2);
  ck_assert_int_eq(err, SUCCESS);
}

Suite* get_parser_test_suite(void) {
  Suite* s = suite_create("parser tests");
  TCase* tc_parser_core = tcase_create("parser core");

  tcase_add_test(tc_parser_core, entered_line_check_test);
  tcase_add_test(tc_parser_core, parse_use_line_test);
  tcase_add_test(tc_parser_core, parse_call_line_test);
  tcase_add_test(tc_parser_core, process_line_test);
  tcase_add_test(tc_parser_core, getopt_test);

  //
  suite_add_tcase(s, tc_parser_core);

  return s;
}
