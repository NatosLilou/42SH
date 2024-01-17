#include "parser.h"

struct ast_rule_until *parse_rule_until(struct lexer *lexer)
{
    struct ast_rule_until *ast = new_ast_rule_until();

    if (lexer_peek(lexer)->type == TOKEN_UNTIL)
    {
        struct token *tok = lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby = parse_compound_list(lexer);
        if (baby)
        {
            ast->compound_list_until = baby;

            if (lexer_peek(lexer)->type == TOKEN_DO)
            {
                tok = lexer_pop(lexer);
                free(tok->value);
                free_token(tok);

                struct ast_compound_list *baby2 = parse_compound_list(lexer);
                if (baby2)
                {
                    ast->compound_list_do = baby2;

                    if (lexer_peek(lexer)->type == TOKEN_DONE)
                    {
                        tok = lexer_pop(lexer);
                        free(tok->value);
                        free_token(tok);

                        return ast;
                    }
                }
            }
        }
    }

    free_ast_rule_until(ast);
    return NULL;
}
