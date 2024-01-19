#include "parser.h"

struct ast_pipeline *parse_pipeline(struct lexer *lexer)
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
        free_token(tok);
    }

    struct ast_command *baby = parse_command(lexer);
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

            baby = parse_command(lexer);
            if (baby)
            {
                add_ast_pipeline(ast, baby);
            }
            else
            {
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

error:
    free_ast_pipeline(ast);
    return NULL;
}
