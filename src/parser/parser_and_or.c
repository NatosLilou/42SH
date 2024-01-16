#include "parser.h"

struct ast_and_or *parse_and_or(struct lexer *lexer)
{
    struct ast_and_or *ast = new_ast_and_or();

    struct ast_pipeline *baby = parse_pipeline(lexer);
    if (baby)
    {
        add_ast_and_or(ast, baby);

        struct token *tok = lexer_peek(lexer);
        while (tok->type == TOKEN_AND_IF || tok->type == TOKEN_OR_IF)
        {
            // TODO: add value token to list
            lexer_pop(lexer);
            free_token(tok);

            tok = lexer_peek(lexer);
            while (tok->type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
                free_token(tok);
                tok = lexer_peek(lexer);
            }

            baby = parse_pipeline(lexer);
            if (baby)
            {
                add_ast_and_or(ast, baby);
            }
            else
            {
                free_ast_and_or(ast);
                return NULL;
            }
        }

        return ast;
    }

    free_ast_and_or(ast);
    return NULL;
}
