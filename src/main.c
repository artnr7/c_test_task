#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>

#include "err.h"
#include "parser.h"

int mode_input(int* mode) {
  int exit = false;
  while (!exit) {
    printf("Choose mode\n");
    printf("Manual: 1\n");
    printf("Auto:   2 (requires 1 argument)\n");
    printf("Mode: ");

    int cnt = scanf("%d", mode);
    char ch = getchar();
    if (ch == 'q' && getchar() == '\n') {
      return EXIT;
    }
    if (cnt == 1 && ch == '\n') {
      switch (*mode) {
        case 1:
          *mode = MANUAL;
          exit = true;
          break;
        case 2:
          *mode = AUTO;
          exit = true;
          break;
        default:
          print_err("WARN", "wrong mode input: use 1 or 2");
          break;
      }
    } else {
      print_err("WARN", "wrong mode input");
      char skip_ch = ' ';
      while ((skip_ch = getchar()) != '\n' && skip_ch != EOF);
    }
  }

  return SUCCESS;
}

int main(int argc, char** argv) {
#if DCHOOSE || DMANUAL
  printf("\nexit 'q'\n\n");
#endif

  int mode = 0;
  int err = SUCCESS;

#ifdef DCHOOSE
  err = mode_input(&mode);
#endif
  if (err == EXIT) {
    return SUCCESS;
  }

#ifdef DMANUAL
  mode = MANUAL;
#elif DAUTO
  mode = AUTO;
#endif

  switch (mode) {
    case MANUAL:
      err = manual_mode();
      break;
    case AUTO:
      err = auto_mode(argc, argv);
      break;
  }

  return err;
}
