#include "parser.h"

static bool is_redir(enum token_type type)
{
    return (type == TOKEN_LESS || type == TOKEN_GREAT || type == TOKEN_DGREAT
            || type == TOKEN_LESSAND || type == TOKEN_GREATAND
            || type == TOKEN_CLOBBER || type == TOKEN_LESSGREAT);
}

static bool is_reserved(enum token_type type)
{
    return (type == TOKEN_IF || type == TOKEN_THEN || type == TOKEN_ELSE
            || type == TOKEN_ELIF || type == TOKEN_FI || type == TOKEN_DO
            || type == TOKEN_DONE || type == TOKEN_WHILE || type == TOKEN_UNTIL
            || type == TOKEN_FOR || type == TOKEN_BANG || type == TOKEN_IN);
}

struct ast_redir *parse_redir(struct lexer *lexer, bool *syntax_error,
                              int loop_stage)
{
    struct ast_redir *ast = new_ast_redir();
    ast->loop_stage = loop_stage;

    bool default_io = true;

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_IO_NUMBER)
    {
        default_io = false;
        lexer_pop(lexer);
        ast->ionumber = atoi(tok->value);
        free(tok->value);
        free_token(tok);
    }

    tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (is_redir(tok->type))
    {
        if (default_io
            && (tok->type == TOKEN_DGREAT || tok->type == TOKEN_GREATAND
                || tok->type == TOKEN_CLOBBER || tok->type == TOKEN_GREAT))
        {
            ast->ionumber = 1;
        }

        ast->redir = tok->type;
        lexer_pop(lexer);
        free_token(tok);

        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        if (tok->type == TOKEN_WORD || is_reserved(tok->type))
        {
            ast->dest = tok->value;
            lexer_pop(lexer);
            free_token(tok);
            return ast;
        }
        *syntax_error = true;
    }

    if (!default_io)
    {
        *syntax_error = true;
    }

error:
    free_ast_redir(ast);
    return NULL;
}
