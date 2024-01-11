#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "../token/token.h"

struct lexer
{
    const char *input; // The command line
    size_t pos; // The current offset inside the input data
};

struct lexer *new_lexer(const char *input);

void free_lexer(struct lexer *lexer);

struct token *token_recognition(struct lexer *lexer);

/**
 * Return first token without moving position
 */
struct token *lexer_peek(struct lexer *lexer);

/**
 * Return first token and moving position to next token
 */
struct token *lexer_pop(struct lexer *lexer);

#endif /* !LEXER_H */
