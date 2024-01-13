#include "eval.h"

int eval_input(struct ast_input *ast_input)
{
    if (ast_input->list == NULL)
    {
        return 0;
    }
    return eval_list(ast_input->list);
}
