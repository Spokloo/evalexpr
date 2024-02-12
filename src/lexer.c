#include "lexer.h"

size_t get_tokens_length(struct token **tokens)
{
    size_t len = 0;
    for (size_t i = 0; tokens[i]; i++)
        len++;

    return len;
}

void free_tokens(struct token **tokens)
{
    for (size_t i = 0; tokens[i]; i++)
        free(tokens[i]);

    free(tokens);
}

struct token *init_token(enum assoc associativity, int priority,
                         int operands_nb, int is_op)
{
    struct token *token = malloc(sizeof(struct token));
    token->associativity = associativity;
    token->priority = priority;
    token->operands_nb = operands_nb;
    token->is_op = is_op;
    return token;
}

static struct token *token_digit(int value)
{
    struct token *digit = init_token(LEFT, 0, 0, 0);
    digit->value = value;
    return digit;
}

static struct token *token_add(void)
{
    struct token *add = init_token(LEFT, 1, 2, 1);
    add->value = TOKEN_ADD;
    return add;
}

static struct token *token_pos(void)
{
    struct token *pos = init_token(RIGHT, 4, 1, 1);
    pos->value = TOKEN_ADD;
    return pos;
}

static struct token *token_minus(void)
{
    struct token *minus = init_token(LEFT, 1, 2, 1);
    minus->value = TOKEN_MINUS;
    return minus;
}

static struct token *token_neg(void)
{
    struct token *neg = init_token(RIGHT, 4, 1, 1);
    neg->value = TOKEN_MINUS;
    return neg;
}

static struct token *token_mult(void)
{
    struct token *mult = init_token(LEFT, 2, 2, 1);
    mult->value = TOKEN_MULT;
    return mult;
}

static struct token *token_div(void)
{
    struct token *div = init_token(LEFT, 2, 2, 1);
    div->value = TOKEN_DIV;
    return div;
}

static struct token *token_mod(void)
{
    struct token *mod = init_token(LEFT, 2, 2, 1);
    mod->value = TOKEN_MOD;
    return mod;
}

static struct token *token_pow(void)
{
    struct token *pow = init_token(RIGHT, 3, 2, 1);
    pow->value = TOKEN_POW;
    return pow;
}

static struct token *token_lpar(struct token **tokens, int is_rpn)
{
    if (is_rpn)
    {
        fprintf(stderr, "Lexical Error: Invalid ( in RPN.\n");
        free_tokens(tokens);
        exit(1);
    }

    struct token *lpar = init_token(RIGHT, 5, 1, 1);
    lpar->value = TOKEN_LPAR;
    return lpar;
}

static struct token *token_rpar(struct token **tokens, int is_rpn)
{
    if (is_rpn)
    {
        fprintf(stderr, "Lexical Error: Invalid ) in RPN.\n");
        free_tokens(tokens);
        exit(1);
    }

    struct token *rpar = init_token(LEFT, 5, 1, 1);
    rpar->value = TOKEN_RPAR;
    return rpar;
}

static struct token *create_add_token(char *input, size_t i, int is_rpn)
{
    struct token *token;
    size_t j = i;
    while (j > 0 && isspace(input[j - 1]))
        j--;

    if (((j > 0 && !isdigit(input[j - 1]) && input[j - 1] != TOKEN_RPAR)
         || j == 0)
        && !is_rpn)
        token = token_pos();
    else
        token = token_add();

    return token;
}

static struct token *create_minus_token(char *input, size_t i, int is_rpn)
{
    struct token *token;
    size_t j = i;
    while (j > 0 && isspace(input[j - 1]))
        j--;

    if (((j > 0 && !isdigit(input[j - 1]) && input[j - 1] != TOKEN_RPAR)
         || j == 0)
        && !is_rpn)
        token = token_neg();
    else
        token = token_minus();

    return token;
}

static int get_digit(char *input, size_t len, size_t *i)
{
    int res = 0;
    char buf[2] = { input[*i], '\0' };

    while (*i < len && isdigit(buf[0]))
    {
        res = res * 10 + atoi(buf);
        (*i)++;
        buf[0] = input[*i];
    }

    (*i)--;
    return res;
}

static struct token **add_token(struct token **tokens, struct token *token)
{
    size_t i = 0;
    while (tokens[i])
        i++;

    tokens[i] = token;
    return tokens;
}

struct token **init_token_list(size_t len)
{
    struct token **tokens = malloc(sizeof(struct token *) * len);
    for (size_t i = 0; i < len; i++)
        tokens[i] = NULL;

    return tokens;
}

struct token **lexer(char *input, size_t len, int is_rpn)
{
    struct token **tokens = init_token_list(len);

    size_t i = 0;
    while (i < len)
    {
        char c = input[i];
        if (isdigit(c))
            tokens = add_token(tokens, token_digit(get_digit(input, len, &i)));
        else if (c == TOKEN_ADD)
            tokens = add_token(tokens, create_add_token(input, i, is_rpn));
        else if (c == TOKEN_MINUS)
            tokens = add_token(tokens, create_minus_token(input, i, is_rpn));
        else if (c == TOKEN_MULT)
            tokens = add_token(tokens, token_mult());
        else if (c == TOKEN_DIV)
            tokens = add_token(tokens, token_div());
        else if (c == TOKEN_MOD)
            tokens = add_token(tokens, token_mod());
        else if (c == TOKEN_POW)
            tokens = add_token(tokens, token_pow());
        else if (c == TOKEN_LPAR)
            tokens = add_token(tokens, token_lpar(tokens, is_rpn));
        else if (c == TOKEN_RPAR)
            tokens = add_token(tokens, token_rpar(tokens, is_rpn));
        else if (!isspace(c))
        {
            fprintf(stderr, "Lexical Error: Invalid character: %c\n", c);
            free_tokens(tokens);
            exit(1);
        }

        i++;
    }

    tokens = realloc(tokens,
                     sizeof(struct token *) * (get_tokens_length(tokens) + 1));
    return tokens;
}
