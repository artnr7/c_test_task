#include "../src/files_work.h"

#include <check.h>

#include "test.h"

START_TEST(open_two_files_for_cp_test) {}
END_TEST

Suite* get_files_work_test_suite(void) {
  Suite* s = suite_create("files_work tests");
  TCase* tc_files_work_core = tcase_create("files_work core");

  tcase_add_test(tc_files_work_core, open_two_files_for_cp_test);

  suite_add_tcase(s, tc_files_work_core);

  return s;
}
