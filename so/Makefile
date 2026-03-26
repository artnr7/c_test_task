NPROC := $(shell expr $$(nproc) / 2)
ifeq ($(shell expr $(NPROC) \< 1), 1)
  NPROC := 1
endif

all:
	gcc -Wall -Werror -Wextra -c -fPIC test.c -o test.o
	gcc -shared test.o -o test.so
