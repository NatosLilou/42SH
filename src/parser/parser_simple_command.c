#include "parser.h"

struct ast_simple_command *parse_simple_command(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_WORD)
    {
        tok = lexer_pop(lexer);
        struct ast_simple_command *ast = new_ast_simple_command(tok->value);

        free_token(tok);

        char *element = parse_element(lexer);
        while (element)
        {
            add_ast_simple_command(ast, element);
            element = parse_element(lexer);
        }

        return ast;
    }

    return NULL;
}
