#include "shunting_yard.h"

static void reverse_tokens(struct token **tokens)
{
    if (!tokens)
        return;

    size_t len = get_tokens_length(tokens);
    if (len == 0)
        return;

    size_t i = 0;
    size_t j = len - 1;

    while (i < j)
    {
        struct token *tmp = tokens[i];
        tokens[i] = tokens[j];
        tokens[j] = tmp;

        i++;
        j--;
    }
}

static void if_operator(struct stack **out, struct stack **op,
                        struct token **tokens, size_t i)
{
    while (!stack_is_empty(*op) && stack_peek(*op)->value != TOKEN_LPAR
           && (stack_peek(*op)->priority > tokens[i]->priority
               || (stack_peek(*op)->priority == tokens[i]->priority
                   && tokens[i]->associativity == LEFT)))
    {
        struct token *op2 = stack_peek(*op);
        *op = stack_pop(*op);
        *out = stack_push(*out, op2);
    }

    *op = stack_push(*op, tokens[i]);
}

static void if_rpar(struct stack **out, struct stack **op)
{
    while (!stack_is_empty(*op) && stack_peek(*op)->value != TOKEN_LPAR)
    {
        struct token *op2 = stack_peek(*op);
        *op = stack_pop(*op);
        *out = stack_push(*out, op2);
    }

    if (stack_is_empty(*op) || stack_peek(*op)->value != TOKEN_LPAR)
    {
        fprintf(stderr, "Syntax Error: Unbalanced parentheses.\n");
        stack_free(*out);
        stack_free(*op);
        exit(2);
    }

    *op = stack_pop(*op);
}

static void handle_token(struct stack **out, struct stack **op,
                         struct token **tokens, size_t *i)
{
    while (tokens[*i])
    {
        if (!tokens[*i]->is_op)
        {
            *out = stack_push(*out, tokens[*i]);
        }
        else
        {
            if (tokens[*i]->value == TOKEN_LPAR)
                *op = stack_push(*op, tokens[*i]);
            else if (tokens[*i]->value == TOKEN_RPAR)
                if_rpar(out, op);
            else
                if_operator(out, op, tokens, *i);
        }

        (*i)++;
    }
}

struct token **shunting_yard(struct token **tokens)
{
    size_t i = 0;
    struct token **rpnify = init_token_list(get_tokens_length(tokens) + 1);
    struct stack *out = NULL;
    struct stack *op = NULL;

    handle_token(&out, &op, tokens, &i);

    while (!stack_is_empty(op))
    {
        struct token *op2 = stack_peek(op);
        if (op2->value == TOKEN_LPAR || op2->value == TOKEN_RPAR)
        {
            fprintf(stderr, "Syntax Error: Unbalanced parentheses.\n");
            stack_free(out);
            stack_free(op);
            exit(2);
        }

        op = stack_pop(op);
        out = stack_push(out, op2);
    }

    size_t j = 0;
    while (!stack_is_empty(out))
    {
        struct token *token = stack_peek(out);
        out = stack_pop(out);
        rpnify[j] = token;
        j++;
    }

    reverse_tokens(rpnify);
    rpnify = realloc(rpnify,
                     sizeof(struct token *) * (get_tokens_length(rpnify) + 1));

    stack_free(out);
    stack_free(op);

    return rpnify;
}
