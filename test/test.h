#ifndef TEST_H_
#define TEST_H_
#include <check.h>

#include "../src/err.h"

#define TEST_TXT "test/manual_mode_test_txt"
#define TESTD "test"
#define STR_TEST_BUF_SIZE 128
#define TCASES_SIZE(tcases) (sizeof(tcases) / sizeof(tcases[0]))

Suite* get_parser_test_suite(void);
Suite* get_files_work_test_suite(void);

#endif  // !DEBUG
