#ifndef SRC_CAT_S21_GREP_H_
#define SRC_CAT_S21_GREP_H_
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  int i, v, c, l, n, h, s, o;
} flags;

int parser(char *str[], flags *flag, int count, char (*t_arr)[250],
           int *count_t, char (*arr_f)[250], int *count_f, char (*arr_f_t)[250],
           int *count_f_t);

void template(char (*t_arr)[250], int *count_t, char (*arr_f_t)[250],
              int *count_f_t);

void FLAG_o(int match, int *o_repeater, int line_number, flags *f,
            regmatch_t *pmatch, char *text, int count_f, char (*arr_f)[250],
            int i);

void FLAG_n(int match, flags *f, int line_number, char *text, int count_f,
            char (*arr_f)[250], int i);

void FLAG_f(flags *f, int count_f, int i, char (*arr_f)[250], int line_matches);

void work_file(int count_f, char (*arr_f)[250], int line_matches, flags *f,
               int i);

void opt(int success, char *text, int *match, int *line_check, flags *f);

void grep(char (*t_arr)[250], int count_t, char (*arr_f)[250], int count_f,
          flags *flag);

void case_e(char **str, int *check_f, int count, char (*t_arr)[250],
            int **count_temp, int *p_temp, int *check);

void case_f(char **str, int *check_f, int count, char (*arr_f_t)[250],
            int **count_t_t, int *p_temp, int *check);

#endif  //  SRC_CAT_S21_GREP_H_