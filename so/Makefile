NPROC := $(shell expr $$(nproc) / 2)
ifeq ($(shell expr $(NPROC) \< 1), 1)
  NPROC := 1
endif

CFILES := $(wildcard *.c)
OBJS := $(CFILES:.c=.o)

all: $(OBJS) test math car

test: test.o utils.o
	gcc -shared test.o utils.o -o test.so

math: mathematic.o utils.o
	gcc -shared mathematic.o utils.o -o mathematic.so
	
car: car.o utils.o
	gcc -shared car.o utils.o -o car.so

%.o: %.c
	gcc -Wall -Werror -Wextra -c -fPIC $< -o $@ 

clean:
	rm -f *.o
