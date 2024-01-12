#include <string.h>

#include "builtin.h"

static void print_string(const char *s, char flags, FILE *sout)
{
    if (flags & 2)
    {
        for (; *s; s++)
        {
            if (*s == '\\')
            {
                switch (s[1])
                {
                case 'n':
                    fprintf(sout, "\n");
                    s++;
                    break;
                case 't':
                    fprintf(sout, "\t");
                    s++;
                    break;
                case '\\':
                    s++;
                /* FALLTHROUGH */
                default:
                    fprintf(sout, "\\");
                    break;
                }
            }
            else
                fprintf(sout, "%c", *s);
        }
    }

    else
        fprintf(sout, "%s", s);
}

int echo(char **argv, FILE *sout)
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
        print_string(*argv, flags, sout);
        ++argv;
        for (; *argv; argv++)
        {
            fprintf(sout, " ");
            print_string(*argv, flags, sout);
        }
    }

    if (!(flags & 1))
        fprintf(sout, "\n");
    fflush(sout);
    return 0;
}
