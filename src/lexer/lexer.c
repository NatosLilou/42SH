#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct lexer *new_lexer(struct io *io)
{
    struct lexer *lex = calloc(1, sizeof(struct lexer)); // /!\ CALLOC NON FREE

    lex->io = io;
    lex->token = NULL;

    return lex;
}

void free_lexer(struct lexer *lexer)
{
    if (lexer->token)
    {
        free_token(lexer->token);
    }
    free(lexer);
}

static int is_delimiter(char c)
{
    return (c == ';' || c == '\n' || c == '\0' || c == ' ');
}

static void lexer_comments(struct lexer *lex)
{
    io_back_end_pop(lex->io);
    char c = io_back_end_peek(lex->io);

    while (c != '\n' && c != '\0')
    {
        io_back_end_pop(lex->io);
        c = io_back_end_peek(lex->io);
    }

    if (c == '\n')
    {
        io_back_end_pop(lex->io);
    }
}

static void lexer_single_quote(struct lexer *lex, struct token *tok)
{
    char *value = calloc(16, sizeof(char)); // /!\ CALLOC NON FREE
    size_t pos = 0;
    size_t size = 16;

    io_back_end_pop(lex->io);
    char c = io_back_end_peek(lex->io);

    while (c != '\'' && c != '\0')
    {
        if (pos >= size - 1) // > for EOF
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

        io_back_end_pop(lex->io);
        c = io_back_end_peek(lex->io);
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
    else if (strcmp(tok->value, "while") == 0)
    {
        tok->type = TOKEN_WHILE;
    }
    else if (strcmp(tok->value, "until") == 0)
    {
        tok->type = TOKEN_UNTIL;
    }
    else if (strcmp(tok->value, "done") == 0)
    {
        tok->type = TOKEN_DONE;
    }
    else if (strcmp(tok->value, "do") == 0)
    {
        tok->type = TOKEN_DO;
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
    bool prev_backslash = false;

    char c = io_back_end_peek(lex->io);

    while (!is_delimiter(c))
    {
        if (pos >= size - 1) // > for EOF
        {
            size += 16;
            value = realloc(value, size);

            for (size_t i = pos; i < size; i++)
            {
                value[i] = '\0';
            }
        }

        if (c == '\\')
        {
            if (prev_backslash)
            {
                prev_backslash = false;
            }
            else
            {
                prev_backslash = true;
                io_back_end_pop(lex->io);
                c = io_back_end_peek(lex->io);
                continue;
            }
        }
        else
        {
            prev_backslash = false;
        }

        if (c == '\'')
        {
            io_back_end_pop(lex->io);
            c = io_back_end_peek(lex->io);
            continue;
        }

        value[pos] = c;
        pos++;

        io_back_end_pop(lex->io);
        c = io_back_end_peek(lex->io);
    }

    // printf("WORD OUT LOOP\n");
    tok->type = TOKEN_WORD;
    tok->value = value;

    lexer_reserved_word(tok);
}

struct token *token_recognition(struct lexer *lex)
{
    struct token *tok = calloc(1, sizeof(struct token));
    tok->value = NULL;

    char c = '\0';

    while ((c = io_back_end_peek(lex->io)) == ' ')
    {
        io_back_end_pop(lex->io);
    }

    // printf("C = %c\n", c);

    if (c == EOF || c == '\0') // EOF
    {
        // printf("TOKEN EOF\n");
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
        free_token(tok);
        return token_recognition(lex);
    }
    else if (c == '\'')
    {
        lexer_single_quote(lex, tok);
    }
    else if (!is_delimiter(c))
    {
        lexer_word(lex, tok);
        return tok;
    }
    else
    {
        tok->type = TOKEN_ERROR;
    }

    io_back_end_pop(lex->io);
    return tok;
}

struct token *lexer_peek(struct lexer *lexer)
{
    if (lexer->token)
    {
        // printf("%d\n", lexer->token->type);
        return lexer->token;
    }
    lexer->token = token_recognition(lexer);
    // printf("%d\n", lexer->token->type);
    return lexer->token;
}

struct token *lexer_pop(struct lexer *lexer)
{
    struct token *tok;
    if (lexer->token)
    {
        tok = lexer->token;
    }
    else
    {
        tok = token_recognition(lexer);
    }

    lexer->token = NULL;
    return tok;
}
