#ifndef EVALUATE_H
#define EVALUATE_H

#include "evalexpr.h"
#include "lexer.h"
#include "stack.h"

int compute_rpn(struct token **tokens);

#endif /* ! EVALUATE_H */
