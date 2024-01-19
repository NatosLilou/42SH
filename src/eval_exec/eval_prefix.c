#include "eval.h"

int eval_prefix(struct ast_prefix *ast)
{
    if (!ast->redir)
    {
        return NULL;
    }
    return eval_redir(ast->redir);
}
