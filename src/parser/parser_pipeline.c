#include "parser.h"

struct ast_pipeline *parse_pipeline(struct lexer *lexer, bool *syntax_error)
{
    struct ast_pipeline *ast = new_ast_pipeline();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_BANG)
    {
        ast->negation = true;
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);
    }

    struct ast_command *baby = parse_command(lexer, syntax_error);
    if (baby)
    {
        add_ast_pipeline(ast, baby);

        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        while (tok->type == TOKEN_PIPE)
        {
            lexer_pop(lexer);
            free_token(tok);

            tok = lexer_peek(lexer);
            if (!tok)
            {
                goto error;
            }
            while (tok->type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
                free_token(tok);
                tok = lexer_peek(lexer);
                if (!tok)
                {
                    goto error;
                }
            }

            baby = parse_command(lexer, syntax_error);
            if (baby)
            {
                add_ast_pipeline(ast, baby);
            }
            else
            {
                *syntax_error = true;
                goto error;
            }

            tok = lexer_peek(lexer);
            if (!tok)
            {
                goto error;
            }
        }

        return ast;
    }

    if (ast->negation)
    {
        *syntax_error = true;
    }

error:
    free_ast_pipeline(ast);
    return NULL;
}
