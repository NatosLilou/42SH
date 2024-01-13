#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct lexer *new_lexer(FILE *stream)
{
    struct lexer *lex = calloc(1, sizeof(struct lexer)); // /!\ CALLOC NON FREE

    lex->stream = stream;
    lex->offset = 0;

    return lex;
}

void free_lexer(struct lexer *lexer)
{
    free(lexer);
}

static int is_delimiter(char c)
{
    return (c == ';' || c == '\n' || c == '\0' || c == ' ');
}

static void lexer_comments(struct lexer *lex)
{
    lex->offset++;
    char c = io_back_end_read(lex->stream, lex->offset);

    while (c != '\n' && c != '\0')
    {
        lex->offset++;
        c = io_back_end_read(lex->stream, lex->offset);
    }

    if (c == '\n')
    {
        lex->offset++;
    }
}

static void lexer_single_quote(struct lexer *lex, struct token *tok)
{
    char *value = calloc(16, sizeof(char)); // /!\ CALLOC NON FREE
    size_t pos = 0;
    size_t size = 16;

    lex->offset++;
    char c = io_back_end_read(lex->stream, lex->offset);

    while (c != '\'' && c != '\0')
    {
        if (pos > size) // > for EOF
        {
            size += 16;
            value = realloc(value, size);

            for (size_t i = pos; i < size; i++)
            {
                value[i] = '\0';
            }
        }

        value[pos] = c;
        pos++;

        lex->offset++;
        c = io_back_end_read(lex->stream, lex->offset);
    }

    if (c == '\0')
    {
        free(value);
        tok->type = TOKEN_ERROR;
    }
    else
    {
        tok->type = TOKEN_WORD;
        tok->value = value;
    }
}

static void lexer_reserved_word(struct token *tok)
{
    if (strcmp(tok->value, "if") == 0)
    {
        tok->type = TOKEN_IF;
    }
    else if (strcmp(tok->value, "then") == 0)
    {
        tok->type = TOKEN_THEN;
    }
    else if (strcmp(tok->value, "else") == 0)
    {
        tok->type = TOKEN_ELSE;
    }
    else if (strcmp(tok->value, "elif") == 0)
    {
        tok->type = TOKEN_ELIF;
    }
    else if (strcmp(tok->value, "fi") == 0)
    {
        tok->type = TOKEN_FI;
    }
    else
    {
        return;
    }
}

static void lexer_word(struct lexer *lex, struct token *tok)
{
    char *value = calloc(16, sizeof(char)); // /!\ CALLOC NON FREE
    size_t pos = 0;
    size_t size = 16;

    char c = io_back_end_read(lex->stream, lex->offset);

    while (!is_delimiter(c))
    {
        if (pos > size) // > for EOF
        {
            size += 16;
            value = realloc(value, size);

            for (size_t i = pos; i < size; i++)
            {
                value[i] = '\0';
            }
        }

        value[pos] = c;
        pos++;

        lex->offset++;
        c = io_back_end_read(lex->stream, lex->offset);
    }
    tok->type = TOKEN_WORD;
    tok->value = value;

    lex->offset--;

    lexer_reserved_word(tok);
}

struct token *token_recognition(struct lexer *lex)
{
    struct token *tok = calloc(1, sizeof(struct token));
    tok->value = NULL;

    char c = io_back_end_read(lex->stream, lex->offset);

    while (c == ' ')
    {
        lex->offset++;
        c = io_back_end_read(lex->stream, lex->offset);
    }

    if (c == '\0') // EOF
    {
        tok->type = TOKEN_EOF;
    }
    else if (c == '\n')
    {
        tok->type = TOKEN_NEWLINE;
    }
    else if (c == ';') // NOT WORKING WITH TOKEN ;;
    {
        tok->type = TOKEN_SEMI;
    }
    else if (c == '#')
    {
        lexer_comments(lex);
        return token_recognition(lex);
    }
    else if (c == '\'')
    {
        lexer_single_quote(lex, tok);
    }
    else if (!is_delimiter(c))
    {
        lexer_word(lex, tok);
    }
    else
    {
        tok->type = TOKEN_ERROR;
    }

    lex->offset++;
    return tok;
}

struct token *lexer_peek(struct lexer *lexer)
{
    long old_offset = lexer->offset;
    struct token *tok = token_recognition(lexer);
    lexer->offset = old_offset;
    return tok;
}

struct token *lexer_pop(struct lexer *lexer)
{
    return token_recognition(lexer);
}
