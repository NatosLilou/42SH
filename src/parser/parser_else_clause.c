#include "parser.h"

struct ast_else_clause *parse_else_clause(struct lexer *lexer)
{
    struct ast_else_clause *ast = new_ast_else_clause();

    if (lexer_peek(lexer)->type == TOKEN_ELSE)
    {
        struct token *tok = lexer_pop(lexer);
        free_token(tok);

        struct ast_compound_list *baby = parse_compound_list(lexer);
        if (baby)
        {
            ast->compound_list_elif = baby;
            return ast;
        }
    }
    else if (lexer_peek(lexer)->type == TOKEN_ELIF)
    {
        struct token *tok = lexer_pop(lexer);
        free_token(tok);

        struct ast_compound_list *baby2 = parse_compound_list(lexer);
        if (baby2)
        {
            ast->compound_list_elif = baby2;

            if (lexer_peek(lexer)->type == TOKEN_THEN)
            {
                tok = lexer_pop(lexer);
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

    free_ast_else_clause(ast);
    return NULL;
}