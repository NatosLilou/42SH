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
    return getenv(var) ? 1 : 0;
}

static void insert_env_var(char *name, char *value)
{
    if (!value)
        setenv(name, "", 0);
    else
        setenv(name, value, 1);
}

static void export_variable(char *name, char *value)
{
    int i = check(name);
    if (i > -1)
    {
        insert_env_var(assigned->name[i], assigned->value[i]);
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
    insert_env_var(name, value);
}

int my_export(char **argv)
{
    ++argv;
    int ret = 0;
    for (; *argv; ++argv)
    {
        char *name = strtok(*argv, "=");
        if (!is_valid_name(name))
        {
            fprintf(stderr, "export : « %s » : bad identifier\n", *argv);
            ret = 1;
        }
        char *value = strtok(NULL, "=");
        if (is_env_var(name))
            insert_env_var(name, value);
        else
            export_variable(name, value);
    }
    return ret;
}
