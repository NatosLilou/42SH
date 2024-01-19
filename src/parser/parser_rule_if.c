#include "parser.h"

struct ast_rule_if *parse_rule_if(struct lexer *lexer)
{
    struct ast_rule_if *ast = new_ast_rule_if();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_IF)
    {
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby = parse_compound_list(lexer);
        if (baby)
        {
            ast->compound_list_if = baby;

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

                struct ast_compound_list *baby2 = parse_compound_list(lexer);
                if (baby2)
                {
                    ast->compound_list_then = baby2;

                    struct ast_else_clause *baby3 = parse_else_clause(lexer);
                    if (baby3)
                    {
                        ast->else_clause = baby3;
                    }
                    tok = lexer_peek(lexer);
                    if (!tok)
                    {
                        goto error;
                    }
                    if (tok->type == TOKEN_FI)
                    {
                        lexer_pop(lexer);
                        free(tok->value);
                        free_token(tok);

                        return ast;
                    }
                }
            }
        }
    }

error:
    free_ast_rule_if(ast);
    return NULL;
}
