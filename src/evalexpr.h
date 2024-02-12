#ifndef EVALEXPR_H
#define EVALEXPR_H

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum assoc
{
    LEFT,
    RIGHT,
};

enum token_val
{
    TOKEN_ADD = '+',
    TOKEN_MINUS = '-',
    TOKEN_MULT = '*',
    TOKEN_DIV = '/',
    TOKEN_MOD = '%',
    TOKEN_POW = '^',
    TOKEN_LPAR = '(',
    TOKEN_RPAR = ')',
};

struct token
{
    int value;
    enum assoc associativity;
    int priority;
    int operands_nb;
    int is_op;
};

void print_tokens(struct token **tokens);

#endif /* ! EVALEXPR_H */
