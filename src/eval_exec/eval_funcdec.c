#include "eval.h"

extern struct assigned_var *assigned;

static int check(char *name)
{
    for (size_t i = 0; i < assigned->pos_fun; i++)
    {
        if (strcmp(name, assigned->fun_name[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int eval_funcdec(struct ast_funcdec *ast)
{
    if (ast->name)
    {
        size_t len_name = strlen(ast->name) + 1;
        char *new_name = calloc(len_name, sizeof(char));
        strcpy(new_name, ast->name);

        int already_in = check(new_name);
        if (already_in == -1)
        {
            assigned->pos_fun += 1;
            assigned->fun_name =
                realloc(assigned->fun_name, assigned->pos_fun * sizeof(char *));
            
            assigned->fun_value =
                realloc(assigned->fun_value, assigned->pos_fun * sizeof(struct ast_shell_command *));
            
            assigned->fun_name[assigned->pos_fun - 1] = new_name;
            assigned->fun_value[assigned->pos_fun - 1] = ast->shell_command;
        }
        else
        {
            free_ast_shell_command((struct ast_shell_command *)assigned->fun_value[already_in]);
            assigned->fun_value[already_in] = ast->shell_command;
            free(new_name);
        }
        return 0;
    }
    err(1,"fun name not a valid identifier");
    return 1;
}
