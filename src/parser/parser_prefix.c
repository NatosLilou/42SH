#include "parser.h"

struct ast_prefix *parse_prefix(struct lexer *lexer)
{
    struct ast_prefix *ast = new_ast_prefix();

    struct ast_redir *baby = parse_redir(lexer);
    if (baby)
    {
        ast->redir = baby;
        return ast;
    }

    free_ast_prefix(ast); // Free if fail

    return NULL;
}
