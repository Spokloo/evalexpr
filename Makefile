CC = gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wvla -pedantic

OBJS=src/evalexpr.o src/evaluate.o src/lexer.o src/shunting_yard.o src/stack.o
NAME=src/evalexpr
BIN=evalexpr
TESTNAME=tests/test.sh

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $^ -o $(BIN)

check:
	./$(TESTNAME)

clean:
	$(RM) src/*.o .ref.out .test.out in $(BIN)
	$(RM) *.html *.css *.gcno *.gcda
