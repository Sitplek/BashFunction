CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11

all: s21_cat

cat.o: s21_cat.c
	$(CC) $(FLAGS) -c s21_cat.c -o s21_cat.o
	clang-format -i --style=Google *.c *.h
s21_cat: cat.o
	$(CC) $(FLAGS) *.o  -o s21_cat


rebuild:
	make clean
	make s21_cat

clean:
	rm -rf *.o s21_cat
	rm -rf *.log