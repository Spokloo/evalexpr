#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "evalexpr.h"
#include "lexer.h"
#include "stack.h"

struct token **shunting_yard(struct token **tokens);

#endif /* ! SHUNTING_YARD_H */
