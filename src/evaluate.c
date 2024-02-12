#include "evaluate.h"

static int add(int a, int b)
{
    return a + b;
}

static int minus(int a, int b)
{
    return a - b;
}

static int mult(int a, int b)
{
    return a * b;
}

static int divide(int a, int b)
{
    if (b == 0)
    {
        fprintf(stderr, "Arithmetical Error: division by zero\n");
        exit(3);
    }

    return a / b;
}

static int mod(int a, int b)
{
    if (b == 0)
    {
        fprintf(stderr, "Arithmetical Error: modulo by zero\n");
        exit(3);
    }

    return a % b;
}

static int my_pow(int a, int b)
{
    if (b < 0)
    {
        fprintf(stderr, "Arithmetical Error: negative power\n");
        exit(3);
    }

    int res = 1;
    for (int i = 0; i < b; i++)
    {
        res *= a;
    }

    return res;
}

struct token *compute_bin(struct token *operator, struct token * a,
                          struct token *b)
{
    switch (operator->value)
    {
    case TOKEN_ADD:
        a->value = add(a->value, b->value);
        break;
    case TOKEN_MINUS:
        a->value = minus(a->value, b->value);
        break;
    case TOKEN_MULT:
        a->value = mult(a->value, b->value);
        break;
    case TOKEN_DIV:
        a->value = divide(a->value, b->value);
        break;
    case TOKEN_MOD:
        a->value = mod(a->value, b->value);
        break;
    case TOKEN_POW:
        a->value = my_pow(a->value, b->value);
        break;
    default:
        break;
    }

    return a;
}

struct token *compute_unary(struct token *operator, struct token * a)
{
    if (operator->value == TOKEN_MINUS)
        a->value = -a->value;

    return a;
}

int compute_rpn(struct token **tokens)
{
    struct stack *s = NULL;
    for (size_t i = 0; i < get_tokens_length(tokens); i++)
    {
        if (!tokens[i]->is_op)
            s = stack_push(s, tokens[i]);
        else
        {
            if (tokens[i]->operands_nb == 2)
            {
                if (stack_size(s) < 2)
                {
                    fprintf(stderr, "Syntax Error. Expected 2 operands.\n");
                    stack_free(s);
                    exit(2);
                }

                struct token *a = stack_peek(s);
                s = stack_pop(s);
                struct token *b = stack_peek(s);
                s = stack_pop(s);
                s = stack_push(s, compute_bin(tokens[i], b, a));
            }
            else
            {
                if (stack_is_empty(s))
                {
                    fprintf(stderr, "Syntax Error. Expected 1 operand.\n");
                    stack_free(s);
                    exit(2);
                }

                struct token *a = stack_peek(s);
                s = stack_pop(s);
                s = stack_push(s, compute_unary(tokens[i], a));
            }
        }
    }

    if (stack_is_empty(s) || stack_size(s) != 1)
    {
        fprintf(stderr, "Syntax Error. Stack size is not 1.\n");
        stack_free(s);
        exit(2);
    }

    int res = stack_peek(s)->value;
    free(s);
    return res;
}
