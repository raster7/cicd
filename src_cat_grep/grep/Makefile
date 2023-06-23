CC = gcc
OS = $(shell uname -s)

CFLAGS = -c -Wall -Werror -Wextra -std=c11 -g
LFLAGS = -lm -lrt -lpthread

all: s21_grep

test:
	bash test0_grep.sh

s21_grep: s21_grep.o
ifeq ($(OS), Linux)
	$(CC) $(LFLAGS) s21_grep.o -o s21_grep
else
	$(CC) s21_grep.o -o s21_grep
endif

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep.o

check:
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *h
ifeq ($(OS), Linux)
	valgrind --track-origins=yes --leak-check=full -s ./s21_grep -v len s21_grep.c
else
	leaks -atExit -- ./s21_grep len s21_grep.c | grep LEAK:
endif

rebuild: clean all check

clean:
	rm -rf  *.o *.txt log_* test_results ./s21_grep

.PHONY: all
		test
		clean
		s21_grep