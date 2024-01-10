#include "echo.h"

#include <stdio.h>
#include <string.h>

static void print_string(const char *s, char flags)
{
    if (flags & 2)
    {
        for (; *s; s++)
        {
            if (*s == '\\')
            {
                switch(s[1])
                {
                case 'n':
                    printf("\n");
                    s++;
                    break;
                case 't':
                    printf("\t");
                    s++;
                    break;
                case '\\':
                    s++;
                /* FALLTHROUGH */
                deafault:
                    printf("\\");
                    break;
                }
            }
            else
                printf("%c", *s);
        }
    }

    else
        printf("%s", s);
}

void echo(char **argv)
{
    char flags = 0;
    for (; *argv; argv++)
    {
        if (!strcmp("-n", *argv))
            flags |= 1;
        else if (!strcmp("-e", *argv))
            flags |= 2;
        else if (!strcmp("-E", *argv))
            flags &= 1;
        else
            break;
    }

    if (*argv)
    {
        print_string(*argv, flags);
        ++argv;
        for (; *argv; argv++)
        {
            printf(" ");
            print_string(*argv, flags);
        }
    }
    if (!(flags & 1))
        puts("");
    fflush(stdout);
}
