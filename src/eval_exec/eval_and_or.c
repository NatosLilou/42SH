#include "eval.h"

extern struct assigned_var *assigned;

int eval_and_or(struct ast_and_or *ast)
{
    if (ast->pipeline == NULL)
    {
        return 0;
    }
    size_t i = 0;
    int res = eval_pipeline(ast->pipeline[i]);
    assigned->exit_code = res;
    i++;
    while (i < ast->pos)
    {
        // /!\  I know it's weird but in C and shell it's switched
        if (ast->op[i - 1] == OP_AND_IF)
        {
            if (res <= -42) // EXIT
            {
                return res;
            }
            res = res || eval_pipeline(ast->pipeline[i]);
            assigned->exit_code = res;
        }
        else
        {
            if (res <= -42) // EXIT
            {
                return res;
            }
            res = res && eval_pipeline(ast->pipeline[i]);
            assigned->exit_code = res;
        }
        i++;
    }
    return res;
}
