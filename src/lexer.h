#ifndef LEXER_H
#define LEXER_H

#include "evalexpr.h"

size_t get_tokens_length(struct token **tokens);
void free_tokens(struct token **tokens);
struct token *init_token(enum assoc associativity, int priority,
                         int operands_nb, int is_op);
struct token **init_token_list(size_t len);
struct token **lexer(char *input, size_t len, int is_rpn);

#endif /* ! LEXER_H */
