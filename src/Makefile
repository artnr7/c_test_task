NPROC := $(shell expr $$(nproc) / 2)
ifeq ($(shell expr $(NPROC) \< 1), 1)
  NPROC := 1
endif

COMPILER := gcc -Wall -Werror -Wextra

ENTRY_POINT := main.c
CORE_FILES := parser.c err.c files_work.c


all:
	@echo "compiling..."
	@$(COMPILER) -DDCHOOSE $(ENTRY_POINT) $(CORE_FILES) -o interpretator

manual_mode:
	@echo "compiling..."
	@$(COMPILER) -DDMANUAL $(ENTRY_POINT) $(CORE_FILES) -o interpretator

auto_mode:
	@echo "compiling..."
	@$(COMPILER) -DDAUTO $(ENTRY_POINT) $(CORE_FILES) -o interpretator

run:
	@./interpretator

build: all

clean:
	@echo "cleaning"
	@rm -f interpretator .tmp .buf test.out ../

rebuild: clean build

r: rebuild run

TEST_FILES := $(wildcard ../test/*.c)
test:
	@echo "compiling.."
	@$(COMPILER) -c -fPIC ../test/mock.c -o mock.o
	@gcc -shared mock.o -o mock.so
	@rm -f mock.o
	@$(COMPILER) $(TEST_FILES) $(CORE_FILES) -o test.out -lcheck
	@echo "running.."
	@./test.out

clangn:
	clang-format -n *.c *.h

clangi:
	clang-format -i *.c *.h
