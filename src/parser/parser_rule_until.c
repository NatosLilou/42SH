#include "parser.h"

struct ast_rule_until *parse_rule_until(struct lexer *lexer, bool *syntax_error,
                                        int loop_stage)
{
    struct ast_rule_until *ast = new_ast_rule_until();
    ast->loop_stage = loop_stage - 1;

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_UNTIL)
    {
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby =
            parse_compound_list(lexer, syntax_error, ast->loop_stage);
        if (baby)
        {
            ast->compound_list_until = baby;

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
                    parse_compound_list(lexer, syntax_error, ast->loop_stage);
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
    free_ast_rule_until(ast);
    return NULL;
}
