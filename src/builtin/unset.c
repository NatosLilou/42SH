#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "builtin.h"

extern struct assigned_var *assigned;

/*  return 1 if the given string is a valid flag
 *  return 2 if it is not a flag
 *  return 3 if it is -- (end of flags)
 *  return 0 otherwise
 */
static int get_flag(char *s, char *flag)
{
    if (!strcmp(s, "--"))
        return 3;

    if (*(s++) == '-')
    {
        for (; *s; s++)
        {
            switch (*s)
            {
            case 'v':
                if (*flag & 2)
                    return 0;
                *flag = 1;
                break;
            case 'f':
                if (*flag & 1)
                    return 0;
                *flag = 2;
                break;
            default:
                return 0;
            }
        }
        return 1;
    }
    return 2;
}

static size_t get_pos_fun(char *name)
{
    size_t i = 0;
    for (; i < assigned->pos_fun; i++)
    {
        if (!strcmp(name, assigned->fun_name[i]))
            break;
    }
    return i;
}

static int unset_functions(char **argv)
{
    int res = 0;
    while (*argv)
    {
        size_t i = get_pos_fun(*argv);
        if (i != assigned->pos_fun)
        {
            free(assigned->fun_name[i]);
            assigned->fun_value[i] = NULL;
            assigned->pos_fun -= 1;
            if (assigned->pos_fun != 0)
            {
                if (i != assigned->pos_fun)
                {
                    assigned->fun_name[i] =
                        assigned->fun_name[assigned->pos_fun];
                    assigned->fun_value[i] =
                        assigned->fun_value[assigned->pos_fun];
                }
                assigned->fun_name = realloc(
                    assigned->fun_name, assigned->pos_fun * sizeof(char *));
                assigned->fun_value = realloc(
                    assigned->fun_value, assigned->pos_fun * sizeof(char *));
            }
        }
        else
            res = 1;
        ++argv;
    }
    return res;
}

static size_t get_pos(char *name)
{
    size_t i = 0;
    for (; i < assigned->pos; i++)
    {
        if (!strcmp(name, assigned->name[i]))
            break;
    }
    return i;
}

static int is_env_var(char *var)
{
    return getenv(var) ? 1 : 0;
}

static int unset_variables(char **argv)
{
    int res = 0;
    while (*argv)
    {
        if (is_env_var(*argv))
        {
            unsetenv(*argv);
            ++argv;
            continue;
        }
        size_t i = get_pos(*argv);
        if (i != assigned->pos)
        {
            free(assigned->name[i]);
            if (assigned->value[i])
                free(assigned->value[i]);
            assigned->pos -= 1;
            if (assigned->pos != 0)
            {
                assigned->name[i] = assigned->name[assigned->pos];
                assigned->value[i] = assigned->value[assigned->pos];
                assigned->name =
                    realloc(assigned->name, assigned->pos * sizeof(char *));
                assigned->value =
                    realloc(assigned->value, assigned->pos * sizeof(char *));
            }
        }
        else
            res = 1;
        ++argv;
    }
    return res;
}

int unset(char **argv)
{
    ++argv;
    char flag = 0;
    for (; *argv; argv++)
    {
        int res = get_flag(*argv, &flag);
        if (!res)
        {
            fprintf(stderr, "unset : usage : unset [-fv] name...\n");
            return 2;
        }
        if (res == 2)
            break;
        if (res == 3)
        {
            argv++;
            break;
        }
    }

    if (flag == 2)
        return unset_functions(argv);
    return unset_variables(argv);
}
