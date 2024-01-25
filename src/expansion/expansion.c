#include "expansion.h"

extern struct assigned_var *assigned;

static bool is_env_var(char *var)
{
    return (strcmp(var, "PWD") == 0 || strcmp(var, "OLDPWD") == 0
            || strcmp(var, "IFS") == 0 || strcmp(var, "UID") == 0
            || strcmp(var, "RANDOM") == 0 || strcmp(var, "$") == 0
            || strcmp(var, "?") == 0);
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
    if (strcmp(var, "*") == 0)
    {
        size_t size = 0;
        for (size_t i = 0; i < assigned->pos_args; i++)
        {
            size += strlen(assigned->args[i]);
        }
        size += assigned->pos_args * 3;

        char *res = calloc(size, sizeof(char));
        res = strcpy(res, assigned->args[0]);

        for (size_t i = 1; i < assigned->pos_args; i++)
        {
            res = strcat(res, " ");
            res = strcat(res, assigned->args[i]);
        }
        return res;
    }
    return NULL;
}


static char *expand_variable(char *value, size_t *pos_value)
{
    (*pos_value)++;

    char *var = calloc(16, sizeof(char));
    size_t size_var = 16;
    size_t pos_var = 0;

    if (value[*pos_value] == '{')
    {
        (*pos_value)++;
        while (value[*pos_value] != '}')
        {
            if (pos_var >= size_var)
            {
                size_var += 16;
                var = realloc(var, size_var);
            }

            var[pos_var] = value[*pos_value];
            pos_var++;
            (*pos_value)++;
        }

        (*pos_value)++;
    }
    else
    {
        while (!is_delimiter(value[*pos_value])
               && !is_first_op(value[*pos_value]) && value[*pos_value] != '"')
        {
            if (pos_var >= size_var)
            {
                size_var += 16;
                var = realloc(var, size_var);
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

    if (is_env_var(var))
    {
        return getenv(var);
    }
    char *res = is_arg(var);
    if (res)
    {
        free(var);
        return res;
    }

    if (assigned)
    {
        for (size_t i = 0; i < assigned->pos; i++)
        {
            // printf("IN FOR ASSIGNED\n");
            if (strcmp(assigned->name[i], var) == 0)
            {
                // printf("IN STRCMP\n");
                res = calloc(strlen(assigned->value[i]) + 1, 1);

                for (size_t j = 0; j < strlen(assigned->value[i]); j++)
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
    return NULL;
}

static char *expand_parameter(char *value)
{
    size_t size_res = strlen(value) + 5;
    char *res = calloc(size_res, sizeof(char));

    bool single_q = false;
    bool double_q = false;
    bool prev_backslash = false;

    size_t pos_value = 0;
    size_t pos_res = 0;

    while (value[pos_value] != '\0')
    {
        if (!single_q && !prev_backslash && value[pos_value] == '$')
        {
            char *var = expand_variable(value, &pos_value);
            if (var)
            {
                size_res += strlen(var);
                res = realloc(res, size_res);

                for (size_t i = pos_res; i < size_res; i++)
                {
                    res[i] = '\0';
                }

                for (size_t i = 0; i < strlen(var); i++)
                {
                    res[pos_res] = var[i];
                    pos_res++;
                }
                free(var);
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

        if (pos_res >= size_res - 1)
        {
            size_res += 16;
            res = realloc(res, size_res);

            for (size_t i = pos_res; i < size_res; i++)
            {
                res[i] = '\0';
            }
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
