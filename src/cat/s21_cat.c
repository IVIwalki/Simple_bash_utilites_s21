#include "s21_cat.h"

int main(int argc, char *argv[]) {
  FILE *fp;
  Flags fl = {0};

  if (argc > 1) {
    if (parser_flags(argc, argv, &fl)) {
      int next_arg = optind;
      while (next_arg < argc) {
        fp = fopen(argv[next_arg], "r");
        if (fp == NULL) {
          fprintf(stderr, "%s No such file or directory", argv[next_arg]);
        } else {
          fclose(cat_print_file(fp, &fl));
        }
        next_arg++;
      }
    }
  }

  return 0;
}

int parser_flags(int argc, char *argv[], Flags *fl) {
  int rez, option_index = -1, flag = 1;
  while ((rez = getopt_long(argc, argv, cat_options, long_options,
                            &option_index)) != -1) {
    switch (rez) {
    case 'b':
      fl->b = 1;
      break;
    case 'e':
      fl->e = 1;
      fl->v = 1;
      break;
    case 'E':
      fl->e = 1;
      break;
    case 'n':
      fl->n = fl->b ? 0 : 1;
      break;
    case 's':
      fl->s = 1;
      break;
    case 't':
      fl->t = 1;
      fl->v = 1;
      break;
    case 'T':
      fl->t = 1;
      break;
    case 'v':
      fl->v = 1;
      break;
    case '?':
      printf("Error Found, wrong argument\n");
      flag = 0;
    };
    option_index = -1;
  }
  return flag;
}

FILE *cat_print_file(FILE *fp, Flags *fl) {
  int current_ch, cnt_line = 0, empty_cnt = 0, last_sym = '\n';
  if (fl->b) {
    fl->n = 0;
  }
  while (1) {
    current_ch = fgetc(fp);
    if (current_ch == EOF) {
      break;
    }
    if (fl->s && current_ch == '\n' && last_sym == '\n') {
      empty_cnt++;
      if (empty_cnt > 1) {
        continue;
      }
    } else {
      empty_cnt = 0;
    }

    if (last_sym == '\n' && ((fl->b && current_ch != '\n') || fl->n)) {
      printf("%6d\t", ++cnt_line);
    }

    if (fl->e) {
      if (fl->b && current_ch == '\n' && last_sym == '\n') {
        printf("      \t");
      }
      if (current_ch == '\n') {
        printf("$");
      }
    }

    if (fl->v) {
      if (current_ch > 127 && current_ch < 160) {
        printf("M-^");
      }
      if (current_ch == 127 ||
          (current_ch < 32 && current_ch != '\n' && current_ch != '\t')) {
        printf("^");
      }
      if ((current_ch < 32 || (current_ch > 126 && current_ch < 160)) &&
          current_ch != '\n' && current_ch != '\t') {
        if (current_ch > 126) {
          current_ch = current_ch - 128 + 64;
        } else {
          current_ch += 64;
        }
      }
    }

    if (fl->t && current_ch == '\t') {
      printf("^");
      current_ch = 'I';
    }

    printf("%c", current_ch);
    last_sym = current_ch;
  }

  return fp;
}