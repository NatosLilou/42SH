#include "parser.h"

struct ast_and_or *parse_and_or(struct lexer *lexer, bool *syntax_error,
                                int loop_stage)
{
    struct ast_and_or *ast = new_ast_and_or();
    ast->loop_stage = loop_stage;

    struct ast_pipeline *baby = parse_pipeline(lexer, syntax_error, loop_stage);
    if (baby)
    {
        add_ast_and_or(ast, baby);

        struct token *tok = lexer_peek(lexer);
        if (!tok || *syntax_error)
        {
            *syntax_error = true;
            goto error;
        }
        while (tok->type == TOKEN_AND_IF || tok->type == TOKEN_OR_IF)
        {
            ast->op[ast->pos - 1] =
                tok->type == TOKEN_AND_IF ? OP_AND_IF : OP_OR_IF;

            lexer_pop(lexer);
            free_token(tok);

            tok = lexer_peek(lexer);
            if (!tok)
            {
                *syntax_error = true;
                goto error;
            }
            while (tok->type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
                free_token(tok);
                tok = lexer_peek(lexer);
                if (!tok)
                {
                    *syntax_error = true;
                    goto error;
                }
            }

            baby = parse_pipeline(lexer, syntax_error, loop_stage);
            if (baby)
            {
                add_ast_and_or(ast, baby);
            }
            else
            {
                *syntax_error = true;
                goto error;
            }
            tok = lexer_peek(lexer);
            if (!tok)
            {
                *syntax_error = true;
                goto error;
            }
        }

        return ast;
    }

error:
    free_ast_and_or(ast);
    return NULL;
}
