#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct lexer *lexer_new(const char *input)
{
    struct lexer *new = calloc(1, sizeof(struct lexer)); // /!\ CALLOC NON FREE

    new->input = input;
    new->pos = 0;

    return new;
}

void lexer_free(struct lexer *lexer)
{
    // free(lexer->input); // /!\ FREE INPUT COMMAND
    free(lexer);
}

static int is_delimiter(char c)
{
    return (c == ';' || c == '\n' || c == '\0' || c == ' ');
}

struct token token_recognition(struct lexer *lex)
{
    struct token new;
    new.value = NULL;

    while (lex->input[lex->pos] == ' ')
    {
        lex->pos++;
    }

    if (lex->input[lex->pos] == '\0') // EOF
    {
        new.type = TOKEN_EOF;
    }
    else if (lex->input[lex->pos] == '\n')
    {
        new.type = TOKEN_NEWLINE;
    }
    else if (!is_delimiter(lex->input[lex->pos]))
    {
        char *value = calloc(16, sizeof(char)); // /!\ CALLOC NON FREE
        size_t index = 0;
        size_t size = 16;
        while (!is_delimiter(lex->input[lex->pos]))
        {
            if (index > size) // > for EOF
            {
                size += 16;
                value = realloc(value, size);
            }

            value[index] = lex->input[lex->pos];
            lex->pos++;
            index++;
        }
        new.type = TOKEN_WORD;
        new.value = value;
    }
    else
    {
        new.type = TOKEN_ERROR;
    }

    return new;
}

struct token lexer_peek(struct lexer *lexer)
{
    size_t old_pos = lexer->pos;
    struct token tok = token_recognition(lexer);
    lexer->pos = old_pos;
    return tok;
}

struct token lexer_pop(struct lexer *lexer)
{
    return token_recognition(lexer);
}
