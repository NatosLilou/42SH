#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "token.h"

struct lexer
{
    const char *input; // The command line
    size_t pos; // The current offset inside the input data
};

struct lexer *lexer_new(const char *input);

void lexer_free(struct lexer *lexer);

struct token token_recognition(struct lexer *lexer);

/**
 * Return first token without moving position
 */
struct token lexer_peek(struct lexer *lexer);

/**
 * Return first token and moving position to next token
 */
struct token lexer_pop(struct lexer *lexer);

#endif /* !LEXER_H */
