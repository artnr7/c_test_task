#include "../src/parser.h"

#include <check.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

#include "../src/err.h"
#include "test.h"

#define STR_TEST_BUF_SIZE 128
#define TCASES_SIZE(tcases) (sizeof(tcases) / sizeof(tcases[0]))

START_TEST(entered_line_check_test) {
  struct {
    const char* line;
    const int exp_err;
  } tcases[] = {
      {"use", WARN_TOO_FEW_ARGUMENTS},
      {"use  fewfewf fewf we", WARN_TOO_MANY_ARGUMENTS},
      {"use fefew.so", SUCCESS},
  };

  for (int i = 0; i < TCASES_SIZE(tcases); ++i) {
    char line_cp[STR_TEST_BUF_SIZE];
    strcpy(line_cp, tcases[i].line);

    strtok(line_cp, " ");
    char* opt = NULL;
    int err = entered_line_check(&opt, tcases[i].line, i);
    ck_assert_int_eq(err, tcases[i].exp_err);
  }
}
END_TEST

START_TEST(parse_use_line_test) {
  struct {
    const char* line;
    const int exp_err;
  } tcases[] = {
      {"use", WARN_SYNTAX},
      {"use  fewfewf fewf we", WARN_SYNTAX},
      {"use fefew.so", FATAL_SO_NOT_LOADED},
      {"use test/mock.so", SUCCESS},
  };

  for (int i = 0; i < TCASES_SIZE(tcases); ++i) {
    char line_cp[STR_TEST_BUF_SIZE];
    strcpy(line_cp, tcases[i].line);

    strtok(line_cp, " ");
    void* dll = NULL;
    int err = parse_use_line(tcases[i].line, i, &dll);
    ck_assert_int_eq(err, tcases[i].exp_err);
  }
}

START_TEST(parse_call_line_test) {
  struct {
    const char* line;
    const int exp_err;
  } tcases[] = {
      {"call", WARN_SYNTAX},
      {"call  fewfewf fewf we", WARN_SYNTAX},
      {"call rfg3fewwe_fn", FATAL_SO_NOT_LOADED},
      {"call ex", SUCCESS},
  };

  for (int i = 0; i < TCASES_SIZE(tcases); ++i) {
    void* dll = NULL;
    if (i == 3) {
      dll = dlopen("./test/mock.so", RTLD_LAZY);
    }
    char line_cp[STR_TEST_BUF_SIZE];
    strcpy(line_cp, tcases[i].line);

    strtok(line_cp, " ");
    int err = parse_call_line(tcases[i].line, i, &dll);
    ck_assert_int_eq(err, tcases[i].exp_err);
    if (i == 3 && dll) {
      dlclose(dll);
    }
  }
}

START_TEST(process_line_test) {
  struct {
    const char* line;
    const int exp_err;
  } tcases[] = {{"call few", FATAL_SO_NOT_LOADED},
                {"use fewfw", FATAL_SO_NOT_LOADED},
                {"use", WARN_SYNTAX},
                {"call use", FATAL_SO_NOT_LOADED},
                {"fewf", WARN_SYNTAX}};

  int line_num = 1;
  for (int i = 0; i < TCASES_SIZE(tcases); ++i) {
    char line_wout_slashn[STR_BUF_SIZE] = {'\0'};
    void* dll = NULL;
    int err =
        process_line(tcases[i].line, line_wout_slashn, &line_num, &dll, MANUAL);
    ck_assert_int_eq(err, tcases[i].exp_err);
  }
}

START_TEST(manual_mode_test) {
#define DDDW "test/manual_mode_test_txt"
  struct {
    const char* filename;
    int exp_err;
  } tcases[] = {
      {DDDW "/success_1.txt", SUCCESS},
      {DDDW "/exit_1.txt", EXIT},
  };

  fclose(stdin);
  for (int i = 0; i < TCASES_SIZE(tcases); ++i) {
    FILE* test_stdin = freopen(tcases[i].filename, "r", stdin);
    ck_assert_ptr_nonnull(test_stdin);
    stdin = test_stdin;

    int err = manual_mode();

    ck_assert_int_eq(err, tcases[i].exp_err);
    fclose(test_stdin);
  }
  stdin = freopen("/dev/tty", "r", stdin);
  ck_assert_ptr_nonnull(stdin);
}
END_TEST

START_TEST(getopt_test) {
  for (int i = -2; i < 3; ++i) {
    if (i == 2) continue;

    int err = getopt(i);
    ck_assert_int_eq(err, FATAL_RUNTIME);
  }
  int err = getopt(2);
  ck_assert_int_eq(err, SUCCESS);
}

START_TEST(auto_mode_test) {
#define EXE "src/interpretator"
#define SCRD "scripts"
  struct {
    const int argc;
    char* argv[8];
    int exp_err;
  } tcases[] = {
      {1, {EXE, SCRD "/test.sc"}, FATAL_RUNTIME},
      {2, {EXE, SCRD "/test.sc"}, FATAL_FUNCTION_NOT_FOUND},
  };

  for (int i = 0; i < TCASES_SIZE(tcases); ++i) {
    int err = auto_mode(tcases[i].argc, tcases[i].argv);
    ck_assert_int_eq(err, tcases[i].exp_err);
  }
}
END_TEST

Suite* get_parser_test_suite(void) {
  Suite* s = suite_create("parser tests");
  TCase* tc_parser_core = tcase_create("parser core");

  tcase_add_test(tc_parser_core, entered_line_check_test);
  tcase_add_test(tc_parser_core, parse_use_line_test);
  tcase_add_test(tc_parser_core, parse_call_line_test);
  tcase_add_test(tc_parser_core, process_line_test);
  tcase_add_test(tc_parser_core, manual_mode_test);
  tcase_add_test(tc_parser_core, getopt_test);
  tcase_add_test(tc_parser_core, auto_mode_test);

  suite_add_tcase(s, tc_parser_core);

  return s;
}
