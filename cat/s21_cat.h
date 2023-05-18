#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct flag {
  int b, e, n, s, t, v;
} flags;

int flags_cat(int argc, char **argv, flags *flag);
void r_file(char **argv, flags *flag);
#endif  // SRC_GREP_S21_GREP_H_