CFILES := $(wildcard *.c)
OBJS := $(CFILES:.c=.o)

all: $(OBJS) test math car clean

test: test.o utils.o
	gcc -shared test.o utils.o -o test.so
	clean

math: mathematic.o utils.o
	gcc -shared mathematic.o utils.o -o mathematic.so
	clean
	
car: car.o utils.o
	gcc -shared car.o utils.o -o car.so
	clean

%.o: %.c
	gcc -Wall -Werror -Wextra -c -fPIC $< -o $@ 

clean:
	rm -f *.o
