NPROC := $(shell expr $$(nproc) / 2)
ifeq ($(shell expr $(NPROC) \< 1), 1)
  NPROC := 1
endif

.PHONY: all build rebuild test clean r t so

COMPILER := gcc -Wall -Werror -Wextra

SRCD := src
TESTD := test
SOD := so

# SRC
ENTRY_POINT := $(SRCD)/main.c
CORE_FS := $(addprefix $(SRCD)/, parser.c err.c files_work.c)
CORE_H_FS :=$(addprefix $(SRCD)/, parser.h err.h files_work.h) 
EXEF := $(SRCD)/interpretator

# TEST
TEST_CORE_FS := $(wildcard $(TESTD)/*.c)
TEST_CORE_H_FS := $(wildcard $(TESTD)/*.h)
TEST_EXEF := $(TESTD)/test.out

# SO
SO_CORE_FS := $(addprefix $(SOD)/, car.c mathematic.c example.c utils.c) 
SO_CORE_H_FS := $(addprefix $(SOD)/, utils.h) 
SO_OBJS := $(SO_CORE_FS:.c=.o) 
SO_SO := $(addprefix $(SOD)/, example.so mathematic.so car.so)

so: $(SO_OBJS) example math car clean

example: $(SOD)/example.o $(SOD)/utils.o
	gcc -shared $(SOD)/example.o $(SOD)/utils.o -o $(SOD)/example.so

math: $(SOD)/mathematic.o $(SOD)/utils.o
	gcc -shared $(SOD)/mathematic.o $(SOD)/utils.o -o $(SOD)/mathematic.so
	
car: $(SOD)/car.o $(SOD)/utils.o
	gcc -shared $(SOD)/car.o $(SOD)/utils.o -o $(SOD)/car.so

%.o: %.c
	$(COMPILER) -c -fPIC $< -o $@ 

clean_so:
	rm -f $(SO_OBJS) 

# MISC
TMP_FS := $(EXEF) $(addprefix $(TESTD)/, mock.so) $(TEST_EXEF)

ALL_CH_FS := $(ENTRY_POINT) $(CORE_FS) $(CORE_H_FS) $(TEST_CORE_FS) $(TEST_CORE_H_FS)

TMPD := .tmpd

all:
	@echo "compiling..."
	@mkdir -p $(TMPD)
	@$(COMPILER) -DDCHOOSE $(ENTRY_POINT) $(CORE_FS) -o $(EXEF)

manual_mode:
	@echo "compiling..."
	@mkdir -p $(TMPD)
	@$(COMPILER) -DDMANUAL $(ENTRY_POINT) $(CORE_FS) -o $(EXEF)

auto_mode:
	@echo "compiling..."
	@mkdir -p $(TMPD)
	@$(COMPILER) -DDAUTO $(ENTRY_POINT) $(CORE_FS) -o $(EXEF)

run:
	@./$(EXEF)

build: all

clean:
	@echo "cleaning.."
	@rm -f $(TMP_FS)
	@rm -rf $(TMPD)

rebuild: clean build

r: rebuild run

test:
	@echo "compiling test.."
	@$(COMPILER) -c -fPIC $(TESTD)/mock.c -o $(TESTD)/mock.o
	@gcc -shared $(TESTD)/mock.o -o $(TESTD)/mock.so
	@rm -f $(TESTD)/mock.o
	@$(COMPILER) $(TEST_CORE_FS) $(CORE_FS) -o $(TEST_EXEF) -lcheck
	@echo "running.."
	@./$(TEST_EXEF)

t: test

clangn:
	clang-format -n $(ALL_CH_FS)

clangi:
	clang-format -i $(ALL_CH_FS)
