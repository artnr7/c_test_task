#include "../src/files_work.h"

#include <stdio.h>

#include "test.h"

START_TEST(open_two_files_for_cp_test) {
  struct {
    const char* src_filename;
    const char* dst_filename;
    const int exp_err;
  } tcases[] = {
      {TEST_TXT "/success_1.txt", TESTD "/mock.txt", SUCCESS},
      {TEST_TXT "/exit_1.txt", TESTD "/mock.txt", SUCCESS},
      {TEST_TXT "/notfound.txt", TESTD "/mock.txt", FATAL_FILE_NOT_FOUND},
  };

  for (size_t i = 0; i < TCASES_SIZE(tcases); ++i) {
    FILE* src = NULL;
    FILE* dst = NULL;

    int err = open_two_files_for_cp(&src, tcases[i].src_filename, &dst,
                                    tcases[i].dst_filename);
    ck_assert_int_eq(err, tcases[i].exp_err);

    if (src) fclose(src);
    if (dst) fclose(dst);
  }
}
END_TEST

START_TEST(copy_files_test) {
  struct {
    const char* src_filename;
    const char* dst_filename;
    const int exp_err;
  } tcases[] = {
      {TEST_TXT "/success_1.txt", TESTD "mock.txt", SUCCESS},
      {TEST_TXT "/exit_1.txt", TESTD "mock.txt", SUCCESS},
      {TEST_TXT "/notfound.txt", TESTD "mock.txt", FATAL_FILES_CANT_BE_COPIED},
  };

  for (size_t i = 0; i < TCASES_SIZE(tcases); ++i) {
    int err = copy_files(tcases[i].src_filename, tcases[i].dst_filename);
    ck_assert_int_eq(err, tcases[i].exp_err);
  }
}
END_TEST

START_TEST(strip_comments_and_join_continuation_lines_test) {
  struct {
    const char* tmp_filename;
    const char* buf_filename;
    const int exp_err;
  } tcases[] = {{TESTD ".tmpmock", TESTD ".bufmock", SUCCESS}};

  for (size_t i = 0; i < TCASES_SIZE(tcases); ++i) {
    int err = strip_comments_and_join_continuation_lines(
        tcases[i].tmp_filename, tcases[i].buf_filename);
    ck_assert_int_eq(err, tcases[i].exp_err);
  }
}
END_TEST

Suite* get_files_work_test_suite(void) {
  Suite* s = suite_create("files_work tests");
  TCase* tc_files_work_core = tcase_create("files_work core");

  tcase_add_test(tc_files_work_core, open_two_files_for_cp_test);
  tcase_add_test(tc_files_work_core, copy_files_test);
  tcase_add_test(tc_files_work_core,
                 strip_comments_and_join_continuation_lines_test);

  suite_add_tcase(s, tc_files_work_core);

  return s;
}
