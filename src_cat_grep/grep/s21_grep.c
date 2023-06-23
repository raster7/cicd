#define _GNU_SOURCE
#include "./s21_grep.h"

unsigned int flag = 0U;
char *pattern = NULL;

int main(int argc, char *argv[]) {
  s21_grep(argc, argv);
  return 0;
}

void stdin_parsing(int argc, char *argv[]) {
  int opt;
  int option_index;
  const char *short_options = "e:ivclnh";

  while ((opt = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'e':
        if (optarg != NULL) {
          flag |= FLAG_REGEXP;
          pattern = optarg;
        } else {
          printf("found size without value\n");
        }
        break;
      case 'i':
        flag |= FLAG_IGNORE_CASE;
        break;
      case 'v':
        flag |= FLAG_INVERT_MATCH;
        break;
      case 'c':
        flag |= FLAG_LAST_CHAR;
        flag |= FLAG_COUNT;
        break;
      case 'l':
        flag |= FLAG_LAST_CHAR;
        flag |= FLAG_FILE_WITH_MATCHES;
        break;
      case 'n':
        flag |= FLAG_LINE_NUMBER;
        break;
      case 'h':
        flag |= FLAG_NO_FILENAME;
        break;
      default: /* ? */
        printf("found unknown option\n");
        break;
    }
  }
}

void del_list(struct string_node *list[]) {
  while (*list != NULL) {
    struct string_node *head = *list;
    (*list) = (*list)->next;
    free(head->str);
    free(head);
  }
}

char *pop_node(struct string_node *list[]) {
  char *ret = NULL;

  if (*list != NULL) {
    struct string_node *head = *list;
    ret = head->str;

    (*list) = (*list)->next;
    free(head);
  }

  return ret;
}

int count_nodes(struct string_node *list[]) {
  struct string_node *current = *list;
  struct string_node *next = NULL;
  int num_of_nodes = 0;

  while (current != NULL) {
    next = current->next;
    num_of_nodes++;
    current = next;
  }

  return num_of_nodes;
}

void add_node(struct string_node *list[], int size, char *origin_str) {
  struct string_node *new_node =
      (struct string_node *)malloc(sizeof(struct string_node));

  new_node->str = (char *)malloc((size + 1) * sizeof(char));
  strcpy(new_node->str, origin_str);

  new_node->next = NULL;

  if (*list == NULL) {
    *list = new_node;
  } else {
    struct string_node *last_node = *list;

    while (last_node->next != NULL) {
      last_node = last_node->next;
    }
    last_node->next = new_node;
  }
}

void ignore_case(char *str, int len) {
  for (int i = 0; i < (int)strlen(pattern); i++) {
    if (pattern[i] >= 'A' && pattern[i] <= 'Z') {
      pattern[i] += 32;
    }
  }
  for (int i = 0; i < len; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] += 32;
    }
  }
}

void find_match(char *str, struct string_node *matches[]) {
  regex_t re;

  char *reference_str = (char *)calloc(strlen(str) + 1, sizeof(char));
  (void)strcpy(reference_str, str);

  if (flag & FLAG_IGNORE_CASE) {
    ignore_case(reference_str, strlen(str));
    regcomp(&re, pattern, REG_EXTENDED | REG_ICASE);
  } else {
    regcomp(&re, pattern, REG_EXTENDED);
  }

  if (regexec(&re, reference_str, (size_t)0, NULL, 0) == 0)
    add_node(matches, strlen(str), str);

  if (reference_str != NULL) free(reference_str);

  regfree(&re);
}

void return_lines_count(int lines_count, int control, char *path) {
  if (control == PRINT_FILE) {
    printf("%s:", path);
  }
  printf("%d\n", lines_count);
}

void return_file_name(char *path) { printf("%s\n", path); }

void return_text(char *string, struct string_node *list[], int control,
                 char *path, int line_number) {
  char *match_str = NULL;
  char *ret = NULL;

  if (control == PRINT_FILE && !(flag & FLAG_NO_FILENAME)) {
    printf("%s:", path);
  }

  if (flag & FLAG_LINE_NUMBER) {
    printf("%d:", line_number);
  }

  while ((match_str = pop_node(list)) != NULL) {
    ret = strstr(string, match_str);

    for (int i = 0; i < (int)strlen(string); i++) printf("%c", string[i]);

    string = string + strlen(match_str) + (ret - string);
    free(match_str);
  }

  for (int i = 0; i < (int)strlen(string); i++) {
    printf("%c", string[i]);
  }

  if (string[(int)strlen(string) - 1] != '\n')
    flag |= FLAG_LAST_CHAR;
  else
    flag &= ~FLAG_LAST_CHAR;
}

void stream_grep(FILE *stream, int control, char *path) {
  struct string_node *match_list = NULL;
  char *stdin_str = NULL;
  size_t len = 0;
  long int lines_count = 0;
  long int line_number = 0;

  while (getline(&stdin_str, &len, stream) != -1) {
    line_number++;
    find_match(stdin_str, &match_list);

    if ((flag & FLAG_FILE_WITH_MATCHES)) {
      if (match_list != NULL) {
        lines_count++;
        continue;
      } else {
        break;
      }
    }

    if (match_list == NULL && (flag & FLAG_INVERT_MATCH)) {
      if (flag & FLAG_COUNT)
        lines_count++;
      else
        return_text(stdin_str, &match_list, control, path, line_number);
    } else if (match_list != NULL && !(flag & FLAG_INVERT_MATCH)) {
      if (flag & FLAG_COUNT)
        lines_count++;
      else
        return_text(stdin_str, &match_list, control, path, line_number);
    }

    del_list(&match_list);
  }

  if ((flag & FLAG_COUNT)) {
    flag &= ~FLAG_LAST_CHAR;
    return_lines_count(lines_count, control, path);
  }

  if ((flag & FLAG_FILE_WITH_MATCHES)) {
    flag &= ~FLAG_LAST_CHAR;
    return_file_name(path);
  }

  free(stdin_str);
}

int what_is(char *path) {
  struct stat stat_buf;
  int ret = NO_FILE_OR_DIR;

  if (stat(path, &stat_buf) == 0) {
    if (S_ISDIR(stat_buf.st_mode)) {
      ret = IS_DIR;
    } else {
      ret = FILE_EXISTS;
    }
  }

  return ret;
}

void get_paths(const char *argv[], struct string_node *path_names[],
               int *flag_num) {
  char *path = NULL;

  if (argv[(*flag_num)] != NULL) {
    while ((path = (char *)argv[(*flag_num)++]) != NULL) {
      int what = 0;
      if ((what = what_is(path)) == FILE_EXISTS) {
        add_node(path_names, (int)strlen(path), path);
      } else {
        char error_str[1000] = "\0";
        fprintf(stderr, "grep: %s: %s", path, strerror(what));
        add_node(path_names, (int)strlen(error_str), error_str);
      }
    }
  }
}

void s21_grep(int argc, char *argv[]) {
  stdin_parsing(argc, argv);
  if (pattern == NULL && !(flag & FLAG_REGEXP)) pattern = argv[optind++];

  struct string_node *path_names = NULL;
  get_paths((const char **)argv, &path_names, &optind);

  if (path_names == NULL) {
    stream_grep(stdin, DO_NOT_PRINT_FILE, NULL);
  } else {
    char *path_name = NULL;
    int num_nodes = count_nodes(&path_names);

    while ((path_name = pop_node(&path_names)) != NULL) {
      if (what_is(path_name) == FILE_EXISTS) {
        FILE *stream = fopen(path_name, "r");

        stream_grep(stream, (num_nodes > 1) ? PRINT_FILE : DO_NOT_PRINT_FILE,
                    path_name);
        fclose(stream);
      } else {
        flag &= ~FLAG_LAST_CHAR;
        fprintf(stderr, "%s\n", path_name);
      }

      free(path_name);
    }

    if (flag & FLAG_LAST_CHAR) printf("\n");
  }
  free(path_names);
}