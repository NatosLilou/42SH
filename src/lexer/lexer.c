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
    else if (strcmp(tok->value, "!") == 0)
    {
        tok->type = TOKEN_BANG;
    }
}

static void lexer_operator_less(struct lexer *lex, struct token *tok,
                                char second)
{
    if (second == '<')
    {
        tok->type = TOKEN_DLESS;
        io_back_end_pop(lex->io);

        char third = io_back_end_peek(lex->io);
        if (third == '-')
        {
            tok->type = TOKEN_DLESSDASH;
            io_back_end_pop(lex->io);
        }
    }
    else if (second == '&')
    {
        tok->type = TOKEN_LESSAND;
        io_back_end_pop(lex->io);
    }
    else if (second == '>')
    {
        tok->type = TOKEN_LESSGREAT;
        io_back_end_pop(lex->io);
    }
    else
    {
        tok->type = TOKEN_LESS;
    }
}

static void lexer_operator_great(struct lexer *lex, struct token *tok,
                                 char second)
{
    if (second == '>')
    {
        tok->type = TOKEN_DGREAT;
        io_back_end_pop(lex->io);
    }
    else if (second == '&')
    {
        tok->type = TOKEN_GREATAND;
        io_back_end_pop(lex->io);
    }
    else if (second == '|')
    {
        tok->type = TOKEN_CLOBBER;
        io_back_end_pop(lex->io);
    }
    else
    {
        tok->type = TOKEN_GREAT;
    }
}

static void lexer_operator(struct lexer *lex, struct token *tok)
{
    char first = io_back_end_pop(lex->io);
    char second = io_back_end_peek(lex->io);
    switch (first)
    {
    case '&':
        if (first == second)
        {
            tok->type = TOKEN_AND_IF;
            io_back_end_pop(lex->io);
        }
        else
        {
            tok->type = TOKEN_AND;
        }
        break;
    case '|':
        if (first == second)
        {
            tok->type = TOKEN_OR_IF;
            io_back_end_pop(lex->io);
        }
        else
        {
            tok->type = TOKEN_PIPE;
        }
        break;
    case ';':
        if (first == second)
        {
            tok->type = TOKEN_DSEMI;
            io_back_end_pop(lex->io);
        }
        else
        {
            tok->type = TOKEN_SEMI;
        }
        break;
    case '<':
        lexer_operator_less(lex, tok, second);
        break;
    case '>':
        lexer_operator_great(lex, tok, second);
        break;
    case '(':
        tok->type = TOKEN_LPAR;
        break;
    case ')':
        tok->type = TOKEN_RPAR;
        break;
    case '\n':
        tok->type = TOKEN_NEWLINE;
        break;
    default:
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
    bool prev_backslash = false;

    char c = io_back_end_peek(lex->io);
    while ((!quoted && !is_delimiter(c) && !is_first_op(c))
           || (quoted && c != EOF && c != '\0'))
    {
        if (c == '\'')
        {
            discard = true;
            quoted = !quoted; // first quote
            io_back_end_pop(lex->io);
            c = io_back_end_peek(lex->io);
            continue;
        }

        if (c == '\\' && !quoted)
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

        if (pos >= size - 1)
        {
            size += 16;
            value = realloc(value, size);
        }
        value[pos] = c;
        pos++;

        io_back_end_pop(lex->io);
        c = io_back_end_peek(lex->io);
    }

    tok->type = TOKEN_WORD;
    tok->value = value;

    if (!discard)
    {
        lexer_reserved_word(tok);
    }

    if (discard && quoted) // Unexpected EOF, syntax error
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
        return tok;
    }

    io_back_end_pop(lex->io); // /!\ pop

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
