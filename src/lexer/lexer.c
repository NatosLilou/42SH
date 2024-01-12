#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

struct lexer *new_lexer(char *input)
{
    struct lexer *new = calloc(1, sizeof(struct lexer)); // /!\ CALLOC NON FREE

    new->input = input;
    new->pos = 0;

    return new;
}

void free_lexer(struct lexer *lexer)
{
    //free(lexer->input); // /!\ FREE INPUT COMMAND
    free(lexer);
}

static int is_delimiter(char c)
{
    return (c == ';' || c == '\n' || c == '\0' || c == ' ' || c == '#');
}

static void lexer_comments(struct lexer *lex)
{
    lex->pos++;
    while (lex->input[lex->pos] != '\n' && lex->input[lex->pos] != '\0')
    {
        lex->pos++;
    }

    if (lex->input[lex->pos] == '\n')
    {
        lex->pos++;
    }
}

static void lexer_single_quote(struct lexer *lex, struct token *tok)
{
    lex->pos++;
    char *value = calloc(16, sizeof(char)); // /!\ CALLOC NON FREE
    size_t pos = 0;
    size_t size = 16;
    while (lex->input[lex->pos] != '\'' && lex->input[lex->pos] != '\0')
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

        value[pos] = lex->input[lex->pos];
        lex->pos++;
        pos++;
    }

    if (lex->input[lex->pos] == '\0')
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
    free(tok->value);
    tok->value = NULL;
}

static void lexer_word(struct lexer *lex, struct token *tok)
{
    char *value = calloc(16, sizeof(char)); // /!\ CALLOC NON FREE
    size_t pos = 0;
    size_t size = 16;
    while (!is_delimiter(lex->input[lex->pos]))
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

        value[pos] = lex->input[lex->pos];
        lex->pos++;
        pos++;
    }
    tok->type = TOKEN_WORD;
    tok->value = value;
    lex->pos--;

    lexer_reserved_word(tok);
}

struct token *token_recognition(struct lexer *lex)
{
    struct token *tok = calloc(1, sizeof(struct token));
    tok->value = NULL;

    while (lex->input[lex->pos] == ' ')
    {
        lex->pos++;
    }

    if (lex->input[lex->pos] == '\0') // EOF
    {
        tok->type = TOKEN_EOF;
    }
    else if (lex->input[lex->pos] == '\n')
    {
        tok->type = TOKEN_NEWLINE;
    }
    else if (lex->input[lex->pos] == ';') // NOT WORKING WITH TOKEN ;;
    {
        tok->type = TOKEN_SEMI;
    }
    else if (lex->input[lex->pos] == '#')
    {
        lexer_comments(lex);
        return token_recognition(lex);
    }
    else if (lex->input[lex->pos] == '\'')
    {
        lexer_single_quote(lex, tok);
    }
    else if (!is_delimiter(lex->input[lex->pos]))
    {
        lexer_word(lex, tok);
    }
    else
    {
        tok->type = TOKEN_ERROR;
    }

    lex->pos++;
    return tok;
}

struct token *lexer_peek(struct lexer *lexer)
{
    size_t old_pos = lexer->pos;
    struct token *tok = token_recognition(lexer);
    lexer->pos = old_pos;
    return tok;
}

struct token *lexer_pop(struct lexer *lexer)
{
    return token_recognition(lexer);
}
