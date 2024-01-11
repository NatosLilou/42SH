#include "parser.h"

struct ast_command *parse_command(struct lexer *lexer)
{
    struct ast_command *ast = new_ast_command();

    struct ast_simple_command *baby = parse_simple_command(lexer);
    if (baby)
    {
        ast->simple_command = baby;
        return ast;
    }

    return NULL;
}
