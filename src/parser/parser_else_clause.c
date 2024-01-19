#include "parser.h"

struct ast_else_clause *parse_else_clause(struct lexer *lexer)
{
    struct ast_else_clause *ast = new_ast_else_clause();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_ELSE)
    {
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby = parse_compound_list(lexer);
        if (baby)
        {
            ast->compound_list_elif = baby;
            return ast;
        }
    }
    else if (tok->type == TOKEN_ELIF)
    {
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby2 = parse_compound_list(lexer);
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
                lexer_pop(lexer);
                free(tok->value);
                free_token(tok);

                struct ast_compound_list *baby3 = parse_compound_list(lexer);
                if (baby3)
                {
                    ast->compound_list_then = baby3;

                    struct ast_else_clause *baby4 = parse_else_clause(lexer);
                    if (baby4)
                    {
                        ast->else_clause = baby4;
                    }

                    return ast;
                }
            }
        }
    }

error:
    free_ast_else_clause(ast);
    return NULL;
}
