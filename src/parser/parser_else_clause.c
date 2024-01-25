#include "parser.h"

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

struct ast_else_clause *parse_else_clause(struct lexer *lexer,
                                          bool *syntax_error)
{
    struct ast_else_clause *ast = new_ast_else_clause();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_ELSE)
    {
        pop_and_free(lexer, tok);

        struct ast_compound_list *baby =
            parse_compound_list(lexer, syntax_error);
        if (baby)
        {
            ast->compound_list_elif = baby;
            return ast;
        }
        *syntax_error = true;
    }
    else if (tok->type == TOKEN_ELIF)
    {
        pop_and_free(lexer, tok);

        struct ast_compound_list *baby2 =
            parse_compound_list(lexer, syntax_error);
        if (baby2)
        {
            ast->compound_list_elif = baby2;

            tok = lexer_peek(lexer);
            if (!tok)
            {
                goto error;
            }
            if (tok->type == TOKEN_THEN)
            {
                pop_and_free(lexer, tok);

                struct ast_compound_list *baby3 =
                    parse_compound_list(lexer, syntax_error);
                if (baby3)
                {
                    ast->compound_list_then = baby3;

                    struct ast_else_clause *baby4 =
                        parse_else_clause(lexer, syntax_error);
                    if (baby4)
                    {
                        ast->else_clause = baby4;
                    }
                    if (*syntax_error)
                    {
                        goto error;
                    }

                    return ast;
                }
            }
        }
        *syntax_error = true;
    }

error:
    free_ast_else_clause(ast);
    return NULL;
}
