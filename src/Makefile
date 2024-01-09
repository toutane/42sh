CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wvla -std=c99 --pedantic -fsanitize=address -g

all: main.o lexer/lexer.o
	$(CC) $(CFLAGS) -o main main.o lexer/lexer.o
