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

// return 1 if the given string is a valid flag, 0 otherwise
static int get_flag(char *s, char *flags)
{
    if (*(s++) == '-')
    {
        int tmp_flags = *flags;
        for (; *s; s++)
        {
            switch (*s)
            {
            case 'n':
                tmp_flags |= 1;
                break;
            case 'e':
                tmp_flags |= 2;
                break;
            case 'E':
                tmp_flags &= 1;
                break;
            default:
                return 0;
            }
        }
        *flags = tmp_flags;
        return 1;
    }
    return 0;
}

int echo(char **argv, FILE *sout)
{
    ++argv;
    char flags = 0;
    for (; *argv && get_flag(*argv, &flags); argv++)
        continue;

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
