CC = gcc
OS = $(shell uname -s)
DIR_TEST_CAT = test_cats/

CFLAGS = -c -Wall -Werror -Wextra -std=c11
LFLAGS = -lm -lrt -lpthread

all: s21_cat

test:
	sh $(DIR_TEST_CAT)test0_cat.sh

s21_cat: s21_cat.o
ifeq ($(OS), Linux)
	$(CC) $(LFLAGS) s21_cat.o -o s21_cat
else
	$(CC) s21_cat.o -o s21_cat
endif

s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat.o

check:
	cppcheck --enable=all --suppress=missingIncludeSystem ../../src/cat/
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *h
ifeq ($(OS), Linux)
	valgrind --track-origins=yes --leak-check=full -s ./s21_cat -te $(DIR_TEST_CAT)main_test.txt
else
	leaks -atExit -- ./s21_cat -te $(DIR_TEST_CAT)main_test.txt
endif

rebuild: clean all

clean:
	rm -rf *.o log* $(DIR_TEST_CAT)log* t_* $(DIR_TEST_CAT)t_* s21_cat $(DIR_TEST_CAT)s21_cat