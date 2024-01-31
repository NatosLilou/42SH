#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "builtin.h"

extern struct assigned_var *assigned;

/*
 * return 0 if the given name is invalid
 * return the length of the name
 */
static int is_valid_name(char *s)
{
    if (('a' <= (*s | 32) && (*s | 32) <= 'z') || *s == '_')
    {
        int i = 1;
        ++s;
        while (*s && *s != '=')
        {
            if (('a' > (*s | 32) || (*s | 32) > 'z') && *s != '_'
                && ('0' > *s || *s > '9'))
                return 0;
            ++s;
            ++i;
        }
        return i;
    }
    return 0;
}

static int check(char *name)
{
    for (size_t i = 0; i < assigned->pos; i++)
    {
        if (strcmp(name, assigned->name[i]) == 0)
            return i;
    }
    return -1;
}

static int is_env_var(char *var)
{
    if ((!strncmp(var, "PWD", 3) || !strncmp(var, "IFS", 3))
        && (!var[3] || var[3] == '='))
        return 3;
    else if (!strncmp(var, "OLDPWD", 6) && (!var[6] || var[6] == '='))
        return 6;
    return 0;
}

static void insert_env_var(char *v, int pos)
{
    if (!v[pos])
        setenv(v, "", 0);
    char *name = strtok(v, "=");
    setenv(name, v + pos + 1, 1);
}

static void insert_variable(char *v, int len)
{
    if (!assigned)
    {
        assigned = malloc(sizeof(struct assigned_var));
        assigned->name = calloc(1, sizeof(char *));
        assigned->value = calloc(1, sizeof(char *));
        assigned->pos = 0;
    }
    char *name = calloc(len + 1, sizeof(char));
    strncpy(name, v, len);
    char *value = NULL;
    if (v[len])
    {
        int vlen = strlen(v + len + 1);
        value = malloc((vlen + 1) * sizeof(char));
        strcpy(value, v + len + 1);
    }

    int already_in = check(name);
    if (already_in == -1)
    {
        assigned->pos += 1;
        assigned->name =
            realloc(assigned->name, assigned->pos * sizeof(char *));
        assigned->value =
            realloc(assigned->value, assigned->pos * sizeof(char *));
        assigned->name[assigned->pos - 1] = name;
        assigned->value[assigned->pos - 1] = value;
    }
    else
    {
        free(name);
        if (value)
        {
            if (assigned->value[already_in])
                free(assigned->value[already_in]);
            assigned->value[already_in] = value;
        }
    }
}

int my_export(char **argv)
{
    ++argv;
    int ret = 0;
    for (; *argv; ++argv)
    {
        int pos = is_env_var(*argv);
        if (pos)
            insert_env_var(*argv, pos);
        else
        {
            int valid = is_valid_name(*argv);
            if (!valid)
            {
                fprintf(stderr, "export : « %s » : bad identifier\n", *argv);
                ret = 1;
            }
            else
                insert_variable(*argv, valid);
        }
    }
    return ret;
}
