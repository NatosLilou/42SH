#include "token.h"

#include <stdlib.h>

void free_token(struct token *tok)
{
    if (tok)
    {
        free(tok);
    }
}
