#include "./s21_cat.h"

int main(int argc, char **argv) {
  int error;

  s21_flags_t flag = {
      .b = 0,
      .e = 0,
      .n = 0,
      .s = 0,
      .t = 0,
      .v = 0,
  };

  error = parsing_stdin(argc, argv, &flag);

  if (error != 1) {
    if (flag.b) flag.n = 0;

    while (optind < argc) {
      if ((error = read_file(argv, &flag)) == 2)
        fprintf(stderr, "%s: %s: %s\n", argv[0], argv[optind], strerror(error));
      optind++;
    }
  }
  return error;
}

int read_file(char **argv, s21_flags_t *flag) {
  int error = 0;
  FILE *file = fopen(argv[optind], "r");

  if (file != NULL) {
    int emp_str_cnt = 0;
    int str_cnt = 0;
    int next_symbol = '\n';

    while (true) {
      int current_symbol = fgetc(file);

      if (current_symbol == EOF) break;

      if (flag->s && current_symbol == '\n' && next_symbol == '\n') {
        emp_str_cnt++;
        if (emp_str_cnt > 1) continue;
      } else {
        emp_str_cnt = 0;
      }

      if (next_symbol == '\n' &&
          ((flag->b && current_symbol != '\n') || flag->n)) {
        str_cnt++;
        printf("%6d\t", str_cnt);
      }

      if (flag->e && current_symbol == '\n') printf("$");

      if (flag->t && current_symbol == '\t') {
        printf("^");
        current_symbol = 'I';
      }

      if (flag->v) {
        if ((current_symbol >= 0x00 && current_symbol < 0x09) ||
            (current_symbol > 0x0A && current_symbol <= 0x1F)) {
          printf("^");
          current_symbol += 64;
        }
        if (current_symbol == 0x7F) {
          printf("^");
          current_symbol = '?';
        }
        if (current_symbol >= 0x80 && current_symbol <= 0x9F) {
          printf("M-^");
          current_symbol -= 64;
        }
        if (current_symbol >= 0xA0 && current_symbol <= 0xFF) {
          printf("M-");
          current_symbol -= 128;
        }
      }

      printf("%c", current_symbol);
      next_symbol = current_symbol;
    }

    fclose(file);

  } else {
    error = 2;
  }
  return error;
}

int parsing_stdin(int argc, char **argv, s21_flags_t *flag) {
  const char *short_options = "bvEnsTet";
  int opt;
  int error = 0;
  int option_index = 0;

  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((opt = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'E':
        flag->e = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case 'b':
        flag->b = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->v = 1;
        flag->t = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      default: /* '?' */
        error = 1;
        break;
    }
  }
  return error;
}