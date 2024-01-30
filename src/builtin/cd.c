#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"

static char *get_real_path(char *tmp)
{
    char *curpath = NULL;
    if (tmp[0] != '/')
    {
        char *pwd = getenv("PWD");
        if (!pwd)
            curpath = getcwd(NULL, 0);
        else
            curpath = strdup(pwd);
        int len_pwd = strlen(curpath);
        int len_tmp = strlen(tmp);
        curpath = realloc(curpath, (len_pwd + len_tmp + 2) * sizeof(char));
        strcat(curpath, "/");
        strcat(curpath, tmp);
    }
    else
        curpath = strdup(tmp);
    int n = 1;
    char **token = malloc(sizeof(char *));
    token[0] = strtok(curpath, "/");
    while (token[n - 1])
    {
        if (!strcmp(token[n - 1], ".."))
        {
            if (n > 1)
                n--;
        }
        else if (*token[n - 1] && strcmp(token[n - 1], "."))
            token = realloc(token, (++n) * sizeof(char *));
        token[n - 1] = strtok(NULL, "/");
    }
    char *res = NULL;
    if (n == 1)
    {
        res = calloc(2, sizeof(char));
        res[0] = '/';
    }
    else
    {
        res = calloc(1, sizeof(char));
        int len = 1;
        for (int i = 0; i + 1 < n; i++)
        {
            len += strlen(token[i]) + 1;
            res = realloc(res, len * sizeof(char));
            strcat(res, "/");
            strcat(res, token[i]);
        }
    }
    free(curpath);
    free(token);
    return res;
}

int cd(char **argv)
{
    ++argv;
    char *tmp = NULL;
    if (!*argv)
    {
        tmp = getenv("HOME");
        if (!tmp)
        {
            fprintf(stderr, "cd : undefined « HOME »\n");
            return 1;
        }
    }
    else if (argv[1])
    {
        fprintf(stderr, "cd : too much arguments\n");
        return 1;
    }
    else
        tmp = *argv;
    if (!*tmp)
        return 0;
    if (!(strcmp(tmp, "-")))
    {
        tmp = getenv("OLDPWD");
        if (!tmp || tmp[0] != '/')
        {
            fprintf(stderr, "cd : undefined « OLDPWD »\n");
            return 1;
        }
    }
    char *curpath = get_real_path(tmp);
    if (chdir(tmp))
    {
        fprintf(stderr, "cd : %s : no file nor folder of this type\n", tmp);
        free(curpath);
        return 1;
    }
    char *oldpwd = getenv("PWD");
    if (!oldpwd)
        oldpwd = "";
    setenv("OLDPWD", oldpwd, 1);
    setenv("PWD", curpath, 1);
    free(curpath);
    return 0;
}
