#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#define buff_max 256

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Flags;

const char *cat_options = "beEnstTv";

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {NULL, 0, NULL, 0}};

int parser_flags(int argc, char *argv[], Flags *fl);
FILE *cat_print_file(FILE *fp, Flags *fl);

#endif
