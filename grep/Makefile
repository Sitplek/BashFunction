CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE
all:build

build: s21_grep

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep
	clang-format -i --style=Google *.c *.h

clean:
	rm -rf *.o s21_grep
	rm -rf *.log

rebuild: clean build