#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "../io_back_end/io_back_end.h"
#include "../token/token.h"

struct lexer
{
    struct io *io;
    struct token *token;
};

struct lexer *new_lexer(struct io *io);

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
