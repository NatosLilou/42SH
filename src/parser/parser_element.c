#include "parser.h"

char *parse_element(struct lexer *lexer)
{
    if (lexer_peek(lexer)->type == TOKEN_WORD)
    {
        struct token *tok = lexer_pop(lexer);

        char *value = tok->value;
        free_token(tok);

        return value;
    }

    return NULL;
}
