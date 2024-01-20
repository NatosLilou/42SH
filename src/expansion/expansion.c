#include "expansion.h"

extern struct assigned_var *assigned;

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
               && !is_first_op(value[*pos_value]))
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
    }

    if (assigned)
    {
        for (size_t i = 0; i <= assigned->pos; i++)
        {
            if (strcmp(assigned->name[i], var) == 0)
            {
                char *res = calloc(strlen(assigned->value[i]), 1);

                for (size_t j = 0; j < strlen(assigned->value[i]); j++)
                {
                    res[j] = assigned->value[i][j];
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
    size_t size_res = strlen(value);
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
            continue;
        }
        else if (!prev_backslash && !double_q && value[pos_value] == '\'')
        {
            single_q = !single_q;
        }
        else if (!prev_backslash && !single_q && value[pos_value] == '"')
        {
            double_q = !double_q;
        }
        else if (!single_q && !prev_backslash && value[pos_value] == '\\')
        {
            prev_backslash = true;
            continue; // TODO DO NOT SKIP
        }

        prev_backslash = false;

        if (pos_res >= size_res)
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
    char *res = calloc(strlen(value), sizeof(char));

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
