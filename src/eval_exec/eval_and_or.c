#include "eval.h"

int eval_and_or(struct ast_and_or *ast)
{
    if (ast->pipeline == NULL)
    {
        return 0;
    }
    size_t i = 0;
    int res = eval_pipeline(ast->pipeline[i]);
    i++;
    while(i < ast->pos)
    {
        // /!\  Je sais c'est bizarre MAIS pour une raison mysterieuse
        if (ast->op[i - 1] == OP_AND_IF)
        {
            if (res == 0) // si le premier Vrai
            {
                res =  eval_pipeline(ast->pipeline[i]);
            }
            return res;
        }
        else
        {
            if (res != 0) // si le premier Faux
            {
                res = eval_pipeline(ast->pipeline[i]);
            }
            return res;
        }
        i++;
    }
    return res;
}
