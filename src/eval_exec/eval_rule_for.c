#define _POSIX_C_SOURCE 200809L

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
    return getenv(name) ? -2 : -1;
}

int eval_rule_for(struct ast_rule_for *ast)
{
    if (!ast->words || !ast->compound_list)
    {
        return 0;
    }
    int res = 0;
    char *name = ast->words[0];
    for (size_t i = 1; i < ast->pos && (res <= ast->loop_stage || res >= 0);
         i++)
    {
        size_t len_name = strlen(name) + 1;
        char *new_name = calloc(len_name, sizeof(char));
        strcpy(new_name, name);

        char *temp = calloc(strlen(ast->words[i]) + 1, sizeof(char));
        strcpy(temp, ast->words[i]);
        char *real_value = expand(temp);
        // ast->words[i] = NULL;
        size_t len_value = strlen(real_value) + 1;
        char *new_value = calloc(len_value, sizeof(char));
        strcpy(new_value, real_value);

        free(real_value);
        int already_in = check(new_name);
        if (already_in == -2)
        {
            setenv(new_name, new_value, 0);
            free(new_name);
            free(new_value);
        }
        else if (already_in == -1)
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
