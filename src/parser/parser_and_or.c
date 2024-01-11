#include "parser.h"

struct ast_and_or *parse_and_or(struct lexer *lexer)
{
    struct ast_and_or *ast = new_ast_and_or();

    struct ast_pipeline *baby = parse_pipeline(lexer);
    if (baby)
    {
        ast->pipeline = baby;
        return ast;
    }

    return NULL;
}
