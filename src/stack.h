#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdlib.h>

#include "evalexpr.h"

struct stack
{
    struct token *data;
    struct stack *next;
};

size_t stack_size(struct stack *s);
int stack_is_empty(struct stack *s);
void stack_free(struct stack *s);
struct stack *stack_push(struct stack *s, struct token *e);
struct stack *stack_pop(struct stack *s);
struct token *stack_peek(struct stack *s);

#endif /* !STACK_H */
