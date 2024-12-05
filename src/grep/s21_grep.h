#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char *pattern;
  int LenPattern;
  int memoryPattern;
} Flags;

// const char *grep_options = "e:ivlncshf:o";

Flags ParserFlags(int argc, char *argv[]);
void patternAdd(Flags *fl, char *pattern);
void AddRegFromFile(Flags *fl, char *filename);
void OutputLine(char *line, int n);
int ProcessFile(Flags fl, char *path, regex_t *reg);
void Output(Flags fl, int argc, char **argv);
void PrintMatch(Flags fl, regex_t *reg, char *line, int line_cnt, char *path);
void matchFlags(Flags fl, int line_cnt, char *path);

#endif