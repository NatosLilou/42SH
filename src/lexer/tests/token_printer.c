#include <stdio.h>

#include "../lexer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);
    struct token token = lexer_pop(lexer);

    while (token.type != TOKEN_EOF)
    {
        if (token.type == TOKEN_WORD)
        {
            printf("WORD %s\n", token.value);
        }
        else if (token.type == TOKEN_SEMI)
        {
            printf("SEMI\n");
        }
        else if (token.type == TOKEN_NEWLINE)
        {
            printf("NEWLINE\n");
        }
        else if (token.type == TOKEN_ERROR)
        {
            printf("ERROR\n");
            break;
        }
        else
        {
            printf("REAL ERROR\n");
            break;
        }

        token = lexer_pop(lexer);
    }

    if (token.type == TOKEN_EOF)
        printf("EOF\n");

    lexer_free(lexer);

    return 0;
}
