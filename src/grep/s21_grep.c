#include "s21_grep.h"

int main(int argc, char **argv) {
  Flags fl = ParserFlags(argc, argv);
  Output(fl, argc, argv);
  free(fl.pattern);

  return 0;
}

Flags ParserFlags(int argc, char *argv[]) {
  Flags fl = {0};
  int opt, option_index = -1;

  struct option long_option[] = {{0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "e:ivlncshf:o", long_option,
                            &option_index)) != -1) {
    switch (opt) {
    case 'e':
      fl.e = 1;
      patternAdd(&fl, optarg);
      break;
    case 'i':
      fl.i = REG_ICASE;
      break;
    case 'v':
      fl.v = 1;
      break;
    case 'c':
      fl.c = 1;
      break;
    case 'l':
      fl.l = 1;
      break;
    case 'n':
      fl.n = 1;
      break;
    case 's':
      fl.s = 1;
      break;
    case 'h':
      fl.h = 1;
      break;
    case 'f':
      fl.f = 1;
      AddRegFromFile(&fl, optarg);
      break;
    case 'o':
      fl.o = 1;
      break;
    }
  }
  if (fl.LenPattern == 0) {
    patternAdd(&fl, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) {
    fl.h = 1;
  }
  return fl;
}

int ProcessFile(Flags fl, char *path, regex_t *reg) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    if (!fl.s) {
      fprintf(stderr, "grep: %s: No such file or directory\n", path);
    }
    return 1;
  }
  int line_cnt = 1;
  int cnt = 0;
  char *line = NULL;
  size_t linelen = 0;
  int read = 0;
  read = getline(&line, &linelen, fp);

  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);

    if ((result == 0 && !fl.v) || (fl.v && result != 0)) {
      if (!fl.c && !fl.l) {
        if (fl.o && !fl.v) {
          PrintMatch(fl, reg, line, line_cnt, path);
        } else {
          matchFlags(fl, line_cnt, path);
          OutputLine(line, read);
        }
      }
      cnt++;
    }
    read = getline(&line, &linelen, fp);
    line_cnt++;
  }

  if (fl.c) {
    if (!fl.h)
      printf("%s:", path);
    if (fl.l && cnt > 0) {
      cnt = 1;
    }
    printf("%d\n", cnt);
  }

  if (fl.l && cnt > 0) {
    cnt = 1;
    printf("%s\n", path);
  }

  free(line);
  fclose(fp);
  return 0;
}

void OutputLine(char *line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') {
    putchar('\n');
  }
}

void Output(Flags fl, int argc, char **argv) {
  regex_t re;
  int miss = regcomp(&re, fl.pattern, REG_EXTENDED | fl.i);
  if (miss)
    fprintf(stderr, "ERROR");
  for (int i = optind; i < argc; i++) {
    ProcessFile(fl, argv[i], &re);
  }
  regfree(&re);
}

void patternAdd(Flags *fl, char *pattern) {
  int len_patt = strlen(pattern);
  if (fl->LenPattern == 0) {
    fl->pattern = malloc(1024 * sizeof(char));
    fl->memoryPattern = 1024;
  }
  if (fl->memoryPattern < fl->LenPattern + len_patt) {
    fl->pattern = realloc(fl->pattern, fl->memoryPattern * 2);
  }
  if (fl->LenPattern != 0) {
    strcat(fl->pattern + fl->LenPattern, "|");
    fl->LenPattern++;
  }
  fl->LenPattern += sprintf(fl->pattern + fl->LenPattern, "(%s)", pattern);
}

void AddRegFromFile(Flags *fl, char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    if (!fl->s) {
      fprintf(stderr, "grep: %s: No such file in directory.", filename);
    }
    exit(1);
  }
  char *line = NULL;
  size_t memlen = 0;
  int read = getline(&line, &memlen, fp);

  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    patternAdd(fl, line);
    read = getline(&line, &memlen, fp);
  }
  free(line);
  fclose(fp);
}

void PrintMatch(Flags fl, regex_t *reg, char *line, int line_cnt, char *path) {
  regmatch_t match;
  int shift = 0;
  while (1) {
    int res = regexec(reg, line + shift, 1, &match, 0);
    if (res != 0) {
      break;
    }
    matchFlags(fl, line_cnt, path);
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[i + shift]);
    }
    putchar('\n');
    shift += match.rm_eo;
  }
}

void matchFlags(Flags fl, int line_cnt, char *path) {
  if (!fl.h)
    printf("%s:", path);
  if (fl.n)
    printf("%d:", line_cnt);
}