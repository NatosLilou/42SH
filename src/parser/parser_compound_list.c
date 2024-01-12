#include "parser.h"

struct ast_compound_list *parse_compound_list(struct lexer *lexer)
{
    struct ast_compound_list *ast = new_ast_compound_list();

    struct ast_and_or *baby = parse_and_or(lexer);
    if (!baby)
    {
        free_ast_compound_list(ast);
        return NULL;
    }
    ast->and_or = baby;

    if (lexer_peek(lexer)->type == TOKEN_SEMI)
    {
        struct token *tok = lexer_pop(lexer);
        free_token(tok);
    }

    while (lexer_peek(lexer)->type == TOKEN_NEWLINE)
    {
        struct token *tok = lexer_pop(lexer);
        free_token(tok);
    }

    return ast;
}
