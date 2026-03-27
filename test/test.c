#include "test.h"

#include <check.h>
#include <stdlib.h>

int main() {
  int success = 0;
  SRunner* runner = NULL;

  runner = srunner_create(NULL);
  srunner_add_suite(runner, parser());

  srunner_run_all(runner, CK_NORMAL);
  success = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (success == 0) ? 0 : 1;
}
