#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "ast/ast.h"


char *expand(char *value);
struct ast_shell_command *expand_func(char *value);

#endif /* !EXPANSION_H */
