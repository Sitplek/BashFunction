#include "s21_grep.h"

int main(int argc, char **argv) {
  flags flag = {0};
  flags *f = &flag;
  char t_arr[argc][250];
  char arr_f[argc][250];
  char arr_f_t[argc][250];
  int count_temp = 0;
  int count_f = 0;
  int count_t_t = 0;
  if (argc >= 3 && parser(argv, f, argc, t_arr, &count_temp, arr_f, &count_f,
                          arr_f_t, &count_t_t) != 1) {
    template(t_arr, &count_temp, arr_f_t, &count_t_t);
    grep(t_arr, count_temp, arr_f, count_f, f);
  }
  return 0;
}

int parser(char **str, flags *f, int count, char (*t_arr)[250], int *count_temp,
           char (*arr_f)[250], int *count_f, char (*arr_f_t)[250],
           int *count_t_t) {
  const char flags[11] = "eivclnhsfo";
  int check = 0;
  int p_temp = 0;
  for (int j = 1; j < count; j++) {
    int check_f = 0;
    if (str[j][0] == '-') {
      for (size_t i = 1; i < strlen(str[j]) && check_f == 0; i++) {
        if (strchr(flags, (int)str[j][i]) == NULL && check_f == 0) {
          check = 1;
          break;
        }
        if (str[j][i] == 'e')
          case_e(str, &check_f, count, t_arr, &count_temp, &p_temp, &check);
        else if (str[j][i] == 'i')
          f->i = 1;
        else if (str[j][i] == 'v')
          f->v = 1;
        else if (str[j][i] == 'c')
          f->c = 1;
        else if (str[j][i] == 'l')
          f->l = 1;
        else if (str[j][i] == 'n')
          f->n = 1;
        else if (str[j][i] == 'h')
          f->h = 1;
        else if (str[j][i] == 's')
          f->s = 1;
        else if (str[j][i] == 'f')
          case_f(str, &check_f, count, arr_f_t, &count_t_t, &p_temp, &check);
        else if (str[j][i] == 'o')
          f->o = 1;
      }
    } else if (p_temp == 1 && str[j][0] != '-' && check_f == 0) {
      strcpy(arr_f[(*count_f)++], str[j]);
      check_f = 1;
      continue;
    } else if (p_temp == 0 && str[j][0] != '-' && check_f == 0) {
      p_temp = 1;
      check_f = 1;
      strcpy(t_arr[(*count_temp)++], str[j]);
      continue;
    }
  }
  if (f->v == 1) f->o = 0;
  return check;
}

void template(char (*t_arr)[250], int *count_temp, char (*arr_f_t)[250],
              int *count_t_t) {
  for (int i = 0; i < (*count_t_t); i++) {
    FILE *fp = fopen(arr_f_t[i], "r");
    if (fp != NULL) {
      char str[250];
      while (fgets(str, 250, fp) != NULL) {
        if (str[strlen(str) - 1] == '\n' && strlen(str) > 1)
          str[strlen(str) - 1] = '\0';
        strcpy(t_arr[(*count_temp)++], str);
      }
    } else
      fclose(fp);
  }
}

void grep(char (*t_arr)[250], int count_temp, char (*arr_f)[250], int count_f,
          flags *f) {
  int cflags = REG_EXTENDED;          // нашел резултат
  if (f->i == 1) cflags = REG_ICASE;  // не смотрит на регистр
  for (int i = 0; i < count_f; i++) {
    FILE *fp = fopen(arr_f[i], "r");
    if (fp != NULL) {
      int line_matches = 0, line_number = 1;
      char text[4096] = {0};
      while (fgets(text, 4095, fp) != NULL) {
        int match = 0, o_repeater = 0;
        regex_t reg;  // структура хранящяя  найденное
        int line_check = 0;
        for (int j = 0; j < count_temp; j++) {
          regmatch_t pmatch[1] = {0};
          regcomp(&reg, t_arr[j], cflags);
          int success = regexec(&reg, text, 1, pmatch, 0);
          opt(success, text, &match, &line_check, f);
          FLAG_o(match, &o_repeater, line_number, f, pmatch, text, count_f,
                 arr_f, i);
          if (line_check == 1) match = 0;
          regfree(&reg);
        }
        FLAG_n(match, f, line_number, text, count_f, arr_f, i);  // вывод текста
        line_matches += match;  // подсчет строк
        line_number++;
      }
      FLAG_f(f, count_f, i, arr_f, line_matches);
      if (f->l == 1 && line_matches > 0)
        printf("%s\n", arr_f[i]);  // вывод имени файла
      work_file(count_f, arr_f, line_matches, f, i);
      fclose(fp);  // закрываем файл
    } else {
      if (f->s != 1)
        fprintf(stderr, "grep: %s: No such file or directory\n", arr_f[i]);
    }
  }
}

void FLAG_o(int match, int *o_repeater, int line_number, flags *f,
            regmatch_t *pmatch, char *text, int count_f, char (*arr_f)[250],
            int i) {
  if (match == 1 && f->o == 1 && f->l != 1 && f->c != 1) {
    if (f->n == 1 && *o_repeater == 0) {  // обработка флагa o
      printf("%d:", line_number);
      *o_repeater = 1;
    }
    for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++)
      printf("%c", text[i]);
    if (pmatch[0].rm_eo != pmatch[0].rm_so) printf("\n");
    if (count_f > 1 && f->h != 1 && (pmatch[0].rm_eo != pmatch[0].rm_so))
      printf("%s:", arr_f[i]);
  }  // вывод имени файла
}

void FLAG_n(int match, flags *f, int line_number, char *text, int count_f,
            char (*arr_f)[250], int i) {
  if (match == 1 && f->l != 1 && f->c != 1 && f->o != 1) {
    if (f->h != 1 && f->l != 1 && count_f > 1) printf("%s:", arr_f[i]);
    if (match == 1 && f->l != 1 && f->c != 1 && f->n == 1)
      printf("%d:", line_number);
    printf("%s", text);
  }
}

void FLAG_f(flags *f, int count_f, int i, char (*arr_f)[250],
            int line_matches) {
  if (f->l == 1 && f->c == 1) {
    if (count_f > 1 && f->h != 1) printf("%s:", arr_f[i]);  // вывод имени файла
    if (line_matches >= 1)
      printf("1\n");
    else
      printf("0\n");
  }
}

void work_file(int count_f, char (*arr_f)[250], int line_matches, flags *f,
               int i) {
  if (f->c == 1 && f->l != 1) {
    if (f->h != 1 && f->l != 1 && count_f > 1)
      printf("%s:", arr_f[i]);     // вывод имени файла
    printf("%d\n", line_matches);  // кол-во строк
  }
}

void opt(int success, char *text, int *match, int *line_check, flags *f) {
  if (strchr(text, '\n') == NULL) strcat(text, "\n");
  if (success == 0 && f->v != 1) *match = 1;
  if (success == 0 && f->v == 1) *line_check = 1;
  if ((success = REG_NOMATCH) && f->v == 1) *match = 1;
}

void case_e(char **str, int *check_f, int count, char (*t_arr)[250],
            int **count_temp, int *p_temp, int *check) {
  for (int j = 1; j < count; j++) {
    for (size_t i = 1; i < strlen(str[j]) && *check_f == 0; i++) {
      if (strlen(str[j]) == i + 1 && *check_f == 0) {
        if (!(j == count - 1)) {
          strcpy(t_arr[(**count_temp)++], str[++j]);
          *p_temp = 1;
          *check_f = 1;
        } else {
          *check = 1;
        }
      } else if (check_f == 0) {
        *check_f = 1;
        strcpy(t_arr[(**count_temp)++], (str[j] + i + 1));
        *p_temp = 1;
      }
    }
  }
}

void case_f(char **str, int *check_f, int count, char (*arr_f_t)[250],
            int **count_t_t, int *p_temp, int *check) {
  for (int j = 1; j < count; j++) {
    for (size_t i = 1; i < strlen(str[j]) && *check_f == 0; i++) {
      if (strlen(str[j]) == i + 1 && *check_f == 0) {
        if (!(j == count - 1)) {
          strcpy(arr_f_t[(**count_t_t)++], str[++j]);
          *check_f = 1;
          *p_temp = 1;
        } else {
          *check = 1;
        }
      } else if (check_f == 0) {
        *check_f = 1;
        strcpy(arr_f_t[(**count_t_t)++], (str[j] + i + 1));
        *p_temp = 1;
      }
    }
  }
}
