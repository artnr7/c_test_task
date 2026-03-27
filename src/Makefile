NPROC := $(shell expr $$(nproc) / 2)
ifeq ($(shell expr $(NPROC) \< 1), 1)
  NPROC := 1
endif

ENTRY_POINT := main.c
UTILS_FILE := parser.c

all:
	@gcc -Wall -Werror -Wextra $(ENTRY_POINT) $(UTILS_FILE) -o interpretator

run:
	@./interpretator ../scripts/test.sc

build: all

clean:
	@rm -rf interpretator

rebuild: clean build

r: rebuild run

