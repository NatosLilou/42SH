#include "parser.h"

char *parse_element(struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_WORD || tok->type == TOKEN_THEN
        || tok->type == TOKEN_ELSE || tok->type == TOKEN_IF
        || tok->type == TOKEN_ELIF
        || tok->type == TOKEN_FI) // Hardcode des reserved words
    {
        struct token *tok = lexer_pop(lexer);

        char *value = tok->value;
        free_token(tok);

        return value;
    }

    return NULL;
}
