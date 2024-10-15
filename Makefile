CC = gcc
CFLAGS = -g -Wall -Werror -std=c99

all: scheduler

scheduler: scheduler.c
	$(CC) $(CFLAGS) -o scheduler scheduler.c schedulerio.c -lm

test01: scheduler
	./scheduler sample_io/input/input-1

test02: scheduler
	./scheduler sample_io/input/input-2

test03: scheduler
	./scheduler sample_io/input/input-3

clean:
	rm -f scheduler *.o *~