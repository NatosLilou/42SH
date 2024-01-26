#include <eval.h>

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

int eval_rule_for(struct ast_rule_for *ast)
{
    if (!ast->words || !ast->compound_list)
    {
        return 0;
    }
    int res = 0;
    char *name = ast->words[0];
    for (size_t i = 1; i < ast->pos; i++)
    {
        size_t len_name = strlen(name) + 1;
        char *new_name = calloc(len_name, sizeof(char));
        strcpy(new_name, name);

        char *real_value = expand(ast->words[i]);
        ast->words[i] = NULL;
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
            free(new_name);
            assigned->value[already_in] =
                realloc(assigned->value[already_in], len_value * sizeof(char));

            strcpy(assigned->value[already_in], new_value);
            free(new_value);
        }
        res = eval_compound_list(ast->compound_list);
    }
    return res;
}