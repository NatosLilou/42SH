#include "eval.h"

int eval_redir(struct ast_redir *ast)
{
    if (!ast->dest)
    {
        return 2;
    }
    return execution_redir(ast);
}
