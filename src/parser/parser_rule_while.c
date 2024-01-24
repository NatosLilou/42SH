#include "parser.h"

struct ast_rule_while *parse_rule_while(struct lexer *lexer, bool *syntax_error)
{
    struct ast_rule_while *ast = new_ast_rule_while();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_WHILE)
    {
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby =
            parse_compound_list(lexer, syntax_error);
        if (baby)
        {
            ast->compound_list_while = baby;

            tok = lexer_peek(lexer);
            if (!tok)
            {
                goto error;
            }
            if (tok->type == TOKEN_DO)
            {
                lexer_pop(lexer);
                free(tok->value);
                free_token(tok);

                struct ast_compound_list *baby2 =
                    parse_compound_list(lexer, syntax_error);
                if (baby2)
                {
                    ast->compound_list_do = baby2;

                    tok = lexer_peek(lexer);
                    if (!tok)
                    {
                        goto error;
                    }
                    if (tok->type == TOKEN_DONE)
                    {
                        lexer_pop(lexer);
                        free(tok->value);
                        free_token(tok);

                        return ast;
                    }
                }
            }
        }

        *syntax_error = true;
    }

error:
    free_ast_rule_while(ast);
    return NULL;
}
