#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define numbers_of_files 2049

typedef struct flag {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} s21_flags_t;

int read_file(char **argv, s21_flags_t *flag);
int parsing_stdin(int argc, char **argv, s21_flags_t *flag);

#endif  // SRC_CAT_S21_CAT_H_