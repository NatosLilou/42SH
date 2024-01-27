#include "eval.h"

int eval_redir(struct ast_redir *ast, struct ast_command *arbre)
{
    if (!ast->dest)
    {
        return 2;
    }
    ast->dest = expand(ast->dest);
    return execution_redir(ast, arbre);
}


int eval_redir_simple(struct ast_redir *ast, struct ast_simple_command *arbre)
{
    if (!ast->dest)
    {
        return 2;
    }
    ast->dest = expand(ast->dest);
    return execution_redir_simple(ast, arbre);
}
