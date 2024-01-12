#include "../execution/execution.h"
#include "eval.h"

int eval(struct ast_input *ast_input)
{
    if (ast_input->list == NULL)
    {
        return 0;
    }
    struct ast_list *ast_list = ast_input->list;
    struct ast_and_or *ast_and_or = ast_list->and_or[0];
    struct ast_pipeline *ast_pipeline = ast_and_or->pipeline;
    struct ast_command *ast_command = ast_pipeline->command;
    struct ast_simple_command *ast_simple_command = ast_command->simple_command;

    return execution(ast_simple_command->commands);
}
