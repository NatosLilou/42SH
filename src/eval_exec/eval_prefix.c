#include "eval.h"

extern struct assigned_var *assigned;

static int check(char *name)
{
    for (size_t i = 0; i < assigned->pos; i++)
    {
        if (strcmp(name, assigned->name[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

void eval_prefix(struct ast_prefix *ast)
{
    if (ast->name)
    {
        size_t len_name = strlen(ast->name) + 1;
        char *new_name = calloc(len_name, sizeof(char));
        strcpy(new_name, ast->name);

        char *real_value = expand(ast->value);
        ast->value = NULL;
        size_t len_value = strlen(real_value) + 1;
        char *new_value = calloc(len_value, sizeof(char));
        strcpy(new_value, real_value);

        free(real_value);
        int already_in = check(new_name);
        if (already_in == -1)
        {
            assigned->pos += 1;
            assigned->name =
                realloc(assigned->name, assigned->pos * sizeof(char *));
            
            assigned->value =
                realloc(assigned->value, assigned->pos * sizeof(char *));
            
            assigned->name[assigned->pos - 1] = new_name;
            assigned->value[assigned->pos - 1] = new_value;
        }
        else
        {
            assigned->value[already_in] =
                realloc(assigned->value[already_in], len_value * sizeof(char));
            strcpy(assigned->value[already_in], new_value);
            free(new_value);
            free(new_name);
        }
    }
}
