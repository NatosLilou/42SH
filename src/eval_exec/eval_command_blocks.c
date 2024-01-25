#include "eval.h"

int eval_command_blocks(struct ast_compound_list *ast)
{
    return eval_compound_list(ast);
}
