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

// Pk parentheses et \n ??
static bool is_first_op(char c)
{
    return (c == '&' || c == '|' || c == ';' || c == '<' || c == '>' || c == '('
            || c == ')' || c == '\n');
}

static bool is_delimiter(char c)
{
    return (c == ' ' || c == '\0' || c == EOF);
}

static void lexer_comments(struct lexer *lex, struct token *tok)
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
        tok->type = TOKEN_NEWLINE;
    }
    else
    {
        tok->type = TOKEN_EOF;
    }
}

/*
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
*/

// TODO => transform in a lookup table
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
    else if (strcmp(tok->value, "for") == 0)
    {
        tok->type = TOKEN_FOR;
    }
    else if (strcmp(tok->value, "in") == 0)
    {
        tok->type = TOKEN_IN;
    }
}

/*
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

    tok->type = TOKEN_WORD;
    tok->value = value;

    lexer_reserved_word(tok);
}
*/

static void lexer_operator(struct lexer *lex, struct token *tok)
{
    char first = io_back_end_peek(lex->io);
    io_back_end_pop(lex->io);
    char second = io_back_end_peek(lex->io);
    switch (first)
    {
    case '&':
        if (first == second)
        {
            tok->type = TOKEN_AND_IF;
        }
        else
        {
            // Cas delimiteur, juste avancer de 1 pour avoir le prochain
        }
        break;
    case '|':
        if (first == second)
        {
            tok->type = TOKEN_OR_IF;
        }
        else
        {}
        break;
    case ';':
        if (first = second)
        {
            tok->type = TOKEN_DSEMI; // not now
        }
        else
        {}
        break;
    case '<':
        /*if (c == first) // DLESS_SLASH TODO too
        {
            tok->type = TOKEN_DLESS;
        }*/
        if (second == '&')
        {
            tok->type = TOKEN_LESSAND;
        }
        else if (second == '>')
        {
            tok->type = TOKEN_LESSGREAT;
        }
        else if (second == '|')
        {
            tok->type = TOKEN_CLOBBER;
        }
        else
        {}
        break;
    case '>':
        if (first == second)
        {
            tok->type = TOKEN_DGREAT;
        }
        else if (second == '&')
        {
            tok->type = TOKEN_GREATAND;
        }
        else
        {}
        break;
    }
}

static void lexer_word(struct lexer *lex, struct token *tok)
{
    char *value = calloc(16, sizeof(char));
    size_t pos = 0;
    size_t size = 16;

    bool discard = false; // presence of quotes
    bool quoted = false; // matching quotes

    char c = io_back_end_peek(lex->io);
    while (!is_delimiter(c))
    {
        if (c == '\'')
        {
            discard = true;
            quoted = false; // first quote
            io_back_end_pop(lex->io);
            c = io_back_end_peek(lex->io);

            while (c != EOF)
            {
                if (c == '\'')
                {
                    quoted = !quoted;
                }
                else // add character
                {
                    if (pos >= size - 1)
                    {
                        size += 16;
                        value = realloc(value, size);
                    }
                    value[pos] = c;
                    pos++;
                }
                io_back_end_pop(lex->io);
                c = io_back_end_peek(lex->io);
            }
        }
        else
        {
            if (pos >= size - 1)
            {
                size += 16;
                value = realloc(value, size);
            }
            io_back_end_pop(lex->io);
            c = io_back_end_peek(lex->io);
        }
    }

    tok->type = TOKEN_WORD;
    tok->value = value;

    if (!discard)
    {
        lexer_reserved_word(tok);
    }
    if (discard and !quoted) // Unexpected EOF, syntax error
    {
        free(tok->value);
        tok = NULL;
    }
}

struct token *token_recognition(struct lexer *lex)
{
    struct token *tok = calloc(1, sizeof(struct token));
    tok->value = NULL;

    char c = io_back_end_peek(lex->io);

    while (c == ' ')
    {
        io_back_end_pop(lex->io);
        c = io_back_end_peek(lex->io);
    }

    if (c == EOF || c == '\0') // EOF
    {
        tok->type = TOKEN_EOF;
    }

    else if (is_first_op(c))
    {
        lexer_operator(lex, tok);
        return tok;
    }

    else if (c == '#')
    {
        lexer_comments(lex, tok);
    }

    else
    {
        lexer_word(lex, tok);
    }

    io_back_end_pop(lex->io);
    return tok;
}

/*
 * OLD LEXER
 *
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
*/

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
