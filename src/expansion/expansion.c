#include "expansion.h"

extern struct assigned_var *assigned;

static bool is_env_var(char *var)
{
    return (strcmp(var, "PWD") == 0 || strcmp(var, "OLDPWD") == 0
            || strcmp(var, "IFS") == 0);
}

static void str_revert(char *s)
{
    int n = strlen(s);
    int i = 0;
    char tmp;
    while (i < n / 2)
    {
        tmp = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = tmp;
        i++;
    }
}

static char *my_itoa(int pos_args)
{
    char *res = calloc(10, sizeof(char));
    int r;
    int i = 0;
    if (pos_args == 0)
    {
        res[0] = '0';
        return res;
    }
    while (pos_args > 0)
    {
        r = pos_args % 10;
        res[i] = r + '0';
        pos_args /= 10;
        i++;
    }
    str_revert(res);
    return res;
}

static char *is_arg(char *var)
{
    int n = atoi(var) - 1;
    if (n >= 0 && (size_t)n < assigned->pos_args)
    {
        char *res = calloc(strlen(assigned->args[n]) + 1, sizeof(char));
        res = strcpy(res, assigned->args[n]);
        return res;
    }
    if (strcmp(var, "#") == 0)
    {
        return my_itoa(assigned->pos_args);
    }
    if (strcmp(var, "*") == 0 || strcmp(var, "@") == 0)
    {
        size_t size = 0;
        for (size_t i = 0; i < assigned->pos_args; i++)
        {
            size += strlen(assigned->args[i]);
        }
        size += 1 + assigned->pos_args * 3;

        char *res = calloc(size, sizeof(char));
        if (assigned->args[0])
        {
            strcpy(res, assigned->args[0]);

            for (size_t i = 1; i < assigned->pos_args; i++)
            {
                res = strcat(res, " ");
                res = strcat(res, assigned->args[i]);
            }
        }
        return res;
    }
    return NULL;
}

static char *is_char(char var)
{
    if (var == '#')
    {
        return my_itoa(assigned->pos_args);
    }
    if (var == '*' || var == '@')
    {
        size_t size = 0;
        for (size_t i = 0; i < assigned->pos_args; i++)
        {
            size += strlen(assigned->args[i]);
        }
        size += 1 + assigned->pos_args * 3;

        char *res = calloc(size, sizeof(char));
        if (assigned->args[0])
        {
            strcpy(res, assigned->args[0]);

            for (size_t i = 1; i < assigned->pos_args; i++)
            {
                res = strcat(res, " ");
                res = strcat(res, assigned->args[i]);
            }
        }
        return res;
    }
    return NULL;
}

static char *is_arg_func(char *var)
{
    int n = atoi(var) - 1;
    if (n >= 0 && (size_t)n < assigned->pos_fun_args)
    {
        char *res = calloc(strlen(assigned->fun_args[n]) + 1, sizeof(char));
        res = strcpy(res, assigned->fun_args[n]);
        return res;
    }
    if (strcmp(var, "#") == 0)
    {
        return my_itoa(assigned->pos_fun_args);
    }
    if (strcmp(var, "*") == 0 || strcmp(var, "@") == 0)
    {
        size_t size = 0;
        for (size_t i = 0; i < assigned->pos_fun_args; i++)
        {
            size += strlen(assigned->fun_args[i]);
        }
        size += 1 + assigned->pos_fun_args * 3;

        char *res = calloc(size, sizeof(char));
        if (assigned->fun_args[0])
        {
            strcpy(res, assigned->fun_args[0]);

            for (size_t i = 1; i < assigned->pos_fun_args; i++)
            {
                res = strcat(res, " ");
                res = strcat(res, assigned->fun_args[i]);
            }
        }
        return res;
    }
    return NULL;
}

static char *is_char_func(char var)
{
    if (var == '#')
    {
        return my_itoa(assigned->pos_fun_args);
    }
    if (var == '*' || var == '@')
    {
        size_t size = 0;
        for (size_t i = 0; i < assigned->pos_fun_args; i++)
        {
            size += strlen(assigned->fun_args[i]);
        }
        size += 1 + assigned->pos_fun_args * 3;

        char *res = calloc(size, sizeof(char));
        if (assigned->fun_args[0])
        {
            strcpy(res, assigned->fun_args[0]);

            for (size_t i = 1; i < assigned->pos_fun_args; i++)
            {
                res = strcat(res, " ");
                res = strcat(res, assigned->fun_args[i]);
            }
        }
        return res;
    }
    return NULL;
}

static bool is_name(char *value)
{
    size_t i = 0;

    if (value[i] >= '0' && value[i] <= '9')
    {
        return false;
    }

    while (value[i] != '\0')
    {
        if (value[i] == '_' || (value[i] >= '0' && value[i] <= '9')
            || (value[i] >= 'a' && value[i] <= 'z')
            || (value[i] >= 'A' && value[i] <= 'Z'))
        {
            i++;
        }
        else
        {
            free(value);
            return false;
        }
    }

    return true;
}

static char *replace_variable(char *var)
{
    if (is_name(var))
    {
        if (assigned)
        {
            for (size_t i = 0; i < assigned->pos; i++)
            {
                // printf("IN FOR ASSIGNED\n");
                if (strcmp(assigned->name[i], var) == 0)
                {
                    size_t l =
                        assigned->value[i] ? strlen(assigned->value[i]) : 0;
                    // printf("IN STRCMP\n");
                    char *res = calloc(l + 1, 1);

                    for (size_t j = 0; j < l; j++)
                    {
                        res[j] = assigned->value[i][j];
                        // fprintf(stdout, "%c\n", res[j]);
                    }

                    free(var);
                    return res;
                }
            }
        }
        free(var);
        char *res = calloc(1, sizeof(char));
        return res;
    }
    return NULL;
}

static bool is_char_name(char value)
{
    return (value == '_' || (value >= '0' && value <= '9')
            || (value >= 'a' && value <= 'z')
            || (value >= 'A' && value <= 'Z'));
}

static char *env_var(char *var)
{
    if (strcmp(var, "?") == 0)
    {
        free(var);
        return my_itoa(assigned->exit_code);
    }

    if (strcmp(var, "UID") == 0)
    {
        free(var);
        return my_itoa(getuid());
    }

    if (strcmp(var, "RANDOM") == 0)
    {
        free(var);
        srand(assigned->seed);
        int r = rand();
        assigned->seed = r;
        return my_itoa(r % 32768);
    }
    else
    {
        return NULL;
    }
}

static bool if_condition(char *var)
{
    return (strcmp(var, "?") == 0) || (strcmp(var, "RANDOM") == 0)
        || (strcmp(var, "UID") == 0);
}

char *special_var(char *var, bool *env)
{
    if (is_env_var(var))
    {
        *env = true;
        char *res = getenv(var);
        if (!res)
        {
            res = "";
        }
        free(var);
        if (!res)
        {
            res = "";
        }
        return res;
    }

    if (if_condition(var))
    {
        return env_var(var);
    }

    char *res = assigned->in_func ? is_arg_func(var) : is_arg(var);
    if (res)
    {
        free(var);
        return res;
    }

    return NULL;
}

static void pospp_free(char *var, size_t *pos_value)
{
    (*pos_value)++;
    free(var);
}

static char *expand_variable(char *value, size_t *pos_value, bool *env)
{
    (*pos_value)++;

    char *var = calloc(16, sizeof(char));
    size_t size_var = 15;
    size_t pos_var = 0;
    char *res = NULL;

    if (value[*pos_value] == '$')
    {
        pospp_free(var, pos_value);
        return my_itoa(getpid());
    }

    if (value[*pos_value] == '{')
    {
        (*pos_value)++;
        while (value[*pos_value] != '}')
        {
            if (pos_var >= size_var)
            {
                var = realloc(var, size_var + 17);
                size_var += 16;
            }

            var[pos_var] = value[*pos_value];
            pos_var++;
            (*pos_value)++;
        }

        (*pos_value)++;
    }
    else
    {
        res = assigned->in_func ? is_char_func(value[*pos_value])
                                : is_char(value[*pos_value]);
        if (res)
        {
            pospp_free(var, pos_value);
            return res;
        }
        if (value[*pos_value] == '?')
        {
            pospp_free(var, pos_value);
            return my_itoa(assigned->exit_code);
        }

        while (is_char_name(value[*pos_value]))
        {
            if (pos_var >= size_var)
            {
                var = realloc(var, size_var + 17);
                size_var += 16;
            }

            var[pos_var] = value[*pos_value];
            pos_var++;
            (*pos_value)++;
            if (var[pos_var - 1] >= '0' && var[pos_var - 1] <= '9')
            {
                break;
            }
        }
    }

    if ((res = special_var(var, env)))
    {
        return res;
    }

    return replace_variable(var);
}

static char *expand_parameter(char *value)
{
    char *res = calloc(25000, sizeof(char));

    bool single_q = false;
    bool double_q = false;
    bool prev_backslash = false;

    size_t pos_value = 0;
    size_t pos_res = 0;

    while (value[pos_value] != '\0')
    {
        if (!single_q && !prev_backslash && value[pos_value] == '$')
        {
            size_t save_pos = pos_value;
            bool env = false;
            char *var = expand_variable(value, &pos_value, &env);

            if (var)
            {
                for (size_t i = 0; i < strlen(var); i++)
                {
                    res[pos_res] = var[i];
                    pos_res++;
                }
                if (!env)
                {
                    free(var);
                }
            }
            else
            {
                for (size_t i = save_pos; i < pos_value; i++)
                {
                    res[pos_res] = value[i];
                    pos_res++;
                }
            }

            prev_backslash = false;
            continue;
        }
        else if (!prev_backslash && !double_q && value[pos_value] == '\'')
        {
            single_q = !single_q;
            prev_backslash = false;
        }
        else if (!prev_backslash && !single_q && value[pos_value] == '"')
        {
            double_q = !double_q;
            prev_backslash = false;
        }
        else if (!single_q && !prev_backslash && value[pos_value] == '\\')
        {
            prev_backslash = true;
        }
        else
        {
            prev_backslash = false;
        }

        res[pos_res] = value[pos_value];
        pos_res++;
        pos_value++;
    }

    return res;
}

static char *free_return(char *res, char *value, bool single_q, bool double_q)
{
    free(value);

    if (single_q || double_q)
    {
        free(res);
        return NULL;
    }

    return res;
}

static bool expand_backslash(char value)
{
    return (value == '$' || value == '`' || value == '"' || value == '\\'
            || value == '\n');
}

static char *quote_removal(char *value, size_t pos_value, size_t pos_res)
{
    char *res = calloc(strlen(value) + 5, sizeof(char));

    bool single_q = false;
    bool double_q = false;
    bool prev_backslash = false;

    while (value[pos_value] != '\0')
    {
        if (value[pos_value] == '\"')
        {
            if (single_q || prev_backslash)
            {
                res[pos_res] = value[pos_value];
                pos_res++;
            }
            else
            {
                double_q = !double_q;
            }

            prev_backslash = false;
        }
        else if (value[pos_value] == '\'')
        {
            if (double_q || prev_backslash)
            {
                res[pos_res] = value[pos_value];
                pos_res++;
            }
            else
            {
                single_q = !single_q;
            }

            prev_backslash = false;
        }
        else if (value[pos_value] == '\\')
        {
            if (single_q || prev_backslash)
            {
                res[pos_res] = value[pos_value];
                pos_res++;

                prev_backslash = false;
            }
            else if (double_q)
            {
                if (expand_backslash(value[pos_value + 1]))
                {
                    prev_backslash = true;
                }
                else
                {
                    res[pos_res] = value[pos_value];
                    pos_res++;

                    prev_backslash = false;
                }
            }
            else
            {
                prev_backslash = true;
            }
        }
        else
        {
            res[pos_res] = value[pos_value];
            pos_res++;

            prev_backslash = false;
        }

        pos_value++;
    }

    return free_return(res, value, single_q, double_q);
}

char *expand(char *value)
{
    char *res = expand_parameter(value);

    free(value);

    res = quote_removal(res, 0, 0);

    return res;
}

struct ast_shell_command *expand_func(char *value)
{
    for (size_t i = 0; i < assigned->pos_fun; i++)
    {
        if (strcmp(assigned->fun_name[i], value) == 0)
        {
            struct ast_shell_command **fun_value =
                (struct ast_shell_command **)assigned->fun_value;
            return (struct ast_shell_command *)fun_value[i];
        }
    }

    return NULL;
}

char *expand_at()
{
    return "UWU";
}
