#include "parser.h"

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

struct ast_rule_if *parse_rule_if(struct lexer *lexer, bool *syntax_error)
{
    struct ast_rule_if *ast = new_ast_rule_if();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_IF)
    {
        pop_and_free(lexer, tok);

        struct ast_compound_list *baby =
            parse_compound_list(lexer, syntax_error);
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
                pop_and_free(lexer, tok);

                struct ast_compound_list *baby2 =
                    parse_compound_list(lexer, syntax_error);
                if (baby2)
                {
                    ast->compound_list_then = baby2;

                    struct ast_else_clause *baby3 =
                        parse_else_clause(lexer, syntax_error);
                    if (baby3)
                    {
                        ast->else_clause = baby3;
                    }
                    if (*syntax_error)
                    {
                        goto error;
                    }
                    tok = lexer_peek(lexer);
                    if (!tok)
                    {
                        goto error;
                    }
                    if (tok->type == TOKEN_FI)
                    {
                        pop_and_free(lexer, tok);
                        return ast;
                    }
                }
            }
        }

        *syntax_error = true;
    }

error:
    free_ast_rule_if(ast);
    return NULL;
}
