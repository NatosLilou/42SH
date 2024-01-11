#include "parser.h"

struct ast_pipeline *parse_pipeline(struct lexer *lexer)
{
    struct ast_pipeline *ast = new_ast_pipeline();

    struct ast_command *baby = parse_command(lexer);
    if (baby)
    {
        ast->command = baby;
        return ast;
    }

    return NULL;
}
