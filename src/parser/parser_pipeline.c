#include "parser.h"

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

static struct ast_pipeline *create_ast(int loop_stage)
{
    struct ast_pipeline *ast = new_ast_pipeline();
    ast->loop_stage = loop_stage;
    return ast;
}

struct ast_pipeline *parse_pipeline(struct lexer *lexer, bool *syntax_error,
                                    int loop_stage)
{
    struct ast_pipeline *ast = create_ast(loop_stage);

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_BANG)
    {
        ast->negation = true;
        pop_and_free(lexer, tok);
    }

    struct ast_command *baby = parse_command(lexer, syntax_error, loop_stage);
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

            baby = parse_command(lexer, syntax_error, loop_stage);
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
