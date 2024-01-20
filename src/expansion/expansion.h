#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"

struct assigned_var
{
    char **name;
    char **value;
    size_t pos;
};

char *expand(char *value);

#endif /* !EXPANSION_H */
