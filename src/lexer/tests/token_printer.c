#include <stdio.h>
#include <stdlib.h>

#include "../lexer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = new_lexer(argv[1]);
    struct token *token = lexer_pop(lexer);

    while (token->type != TOKEN_EOF)
    {
        if (token->type == TOKEN_WORD)
        {
            printf("WORD %s\n", token->value);
        }
        else if (token->type == TOKEN_SEMI)
        {
            printf("SEMI\n");
        }
        else if (token->type == TOKEN_NEWLINE)
        {
            printf("NEWLINE\n");
        }
        else if (token->type == TOKEN_ERROR)
        {
            printf("ERROR\n");
            break;
        }
        else if (token->type == TOKEN_IF)
        {
            printf("IF\n");
        }
        else if (token->type == TOKEN_THEN)
        {
            printf("THEN\n");
        }
        else if (token->type == TOKEN_ELSE)
        {
            printf("ELSE\n");
        }
        else if (token->type == TOKEN_ELIF)
        {
            printf("ELIF\n");
        }
        else if (token->type == TOKEN_FI)
        {
            printf("FI\n");
        }
        else
        {
            printf("REAL ERROR\n");
            break;
        }

        if (token->value)
        {
            free(token->value);
        }
        free_token(token);
        token = lexer_pop(lexer);
    }

    if (token->type == TOKEN_EOF)
        printf("EOF\n");

    if (token->value)
    {
        free(token->value);
    }
    free_token(token);
    free_lexer(lexer);

    return 0;
}
