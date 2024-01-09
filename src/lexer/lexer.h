#ifndef LEXER_H
#define LEXER_H

#include "token.h"

struct lexer
{
    const char *input; // The command line
    size_t pos; // The current offset inside the input data
};

struct lexer *lexer_new(const char *input);

void lexer_free(struct lexer *lexer);

#endif /* !LEXER_H */
