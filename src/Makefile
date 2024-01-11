CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wvla -std=c99 --pedantic -fsanitize=address -g

all: main.o lexer/lexer.o parser/parser.o ast/ast.o io_backend/io_backend.o
	$(CC) $(CFLAGS) -o main main.o lexer/lexer.o parser/parser.o ast/ast.o io_backend/io_backend.o
