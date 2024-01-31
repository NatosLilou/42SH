#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        if (lexer->token->value)
        {
            free(lexer->token->value);
        }
        free_token(lexer->token);
    }
    if (lexer->next_token)
    {
        if (lexer->next_token->value)
        {
            free(lexer->next_token->value);
        }
        free_token(lexer->next_token);
    }
    free(lexer);
}

bool is_first_op(char c)
{
    return (c == '&' || c == '|' || c == ';' || c == '<' || c == '>' || c == '('
            || c == ')' || c == '\n');
}

bool is_delimiter(char c)
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
    else if (strcmp(tok->value, "case") == 0)
    {
        tok->type = TOKEN_CASE;
    }
    else if (strcmp(tok->value, "esac") == 0)
    {
        tok->type = TOKEN_ESAC;
    }
    else if (strcmp(tok->value, "in") == 0)
    {
        tok->type = TOKEN_IN;
    }
    else if (strcmp(tok->value, "!") == 0)
    {
        tok->type = TOKEN_BANG;
    }
    else if (strcmp(tok->value, "{") == 0)
    {
        tok->type = TOKEN_LBRACE;
    }
    else if (strcmp(tok->value, "}") == 0)
    {
        tok->type = TOKEN_RBRACE;
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
    if (first == '\n')
    {
        tok->type = TOKEN_NEWLINE;
        return;
    }
    char second = io_back_end_peek(lex->io);
    switch (first)
    {
    case '&':
        tok->type = first == second ? TOKEN_AND_IF : TOKEN_AND;
        if (first == second)
        {
            io_back_end_pop(lex->io);
        }
        break;
    case '|':
        tok->type = first == second ? TOKEN_OR_IF : TOKEN_PIPE;
        if (first == second)
        {
            io_back_end_pop(lex->io);
        }
        break;
    case ';':
        tok->type = first == second ? TOKEN_DSEMI : TOKEN_SEMI;
        if (first == second)
        {
            io_back_end_pop(lex->io);
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
    default:
        break;
    }
}

static void lexer_io_number(struct lexer *lex, struct token *tok)
{
    if (tok->type != TOKEN_WORD)
    {
        return;
    }

    char c = io_back_end_peek(lex->io);
    if (c == '<' || c == '>')
    {
        size_t i = 0;
        while (tok->value[i] != '\0')
        {
            if (tok->value[i] >= '0' && tok->value[i] <= '9')
            {
                i++;
            }
            else
            {
                return;
            }
        }

        tok->type = TOKEN_IO_NUMBER;
    }
}

static struct token *lexer_word(struct lexer *lex, struct token *tok)
{
    char *value = calloc(16, sizeof(char));
    size_t pos = 0;
    size_t size = 16;

    bool discard = false; // presence of quotes
    bool single_q = false;
    bool double_q = false;
    bool prev_backslash = false;

    char c = io_back_end_peek(lex->io);
    while (prev_backslash || (!single_q && !is_delimiter(c) && !is_first_op(c))
           || ((single_q || double_q) && c != EOF && c != '\0'))
    {
        if (c == '\'' && !double_q)
        {
            discard = true;
            single_q = !single_q; // single quote status
        }

        else if (c == '"' && !single_q && !prev_backslash)
        {
            discard = true;
            double_q = !double_q; // double quote status
        }

        if (c == '\\' && !single_q)
        {
            prev_backslash = !prev_backslash;
        }
        else
        {
            prev_backslash = false;
        }

        if (pos >= size - 1)
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

    tok->type = TOKEN_WORD;
    tok->value = value;

    if (!discard)
    {
        lexer_reserved_word(tok);
        lexer_io_number(lex, tok);
    }

    if (discard && (single_q || double_q)) // Unexpected EOF, syntax error
    {
        free(tok->value);
        free_token(tok);
        tok = NULL;
    }

    return tok;
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
        tok = lexer_word(lex, tok);
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

struct token *lexer_peek_ahead(struct lexer *lexer)
{
    if (lexer->next_token)
    {
        return lexer->next_token;
    }
    if (!lexer->token)
    {
        lexer->token = token_recognition(lexer);
    }
    lexer->next_token = token_recognition(lexer);
    return lexer->next_token;
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

    lexer->token = lexer->next_token;
    lexer->next_token = NULL;
    return tok;
}
