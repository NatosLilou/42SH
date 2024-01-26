#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "lexer/lexer.h"

char *expand(char *value);
char *expand_at();
struct ast_shell_command *expand_func(char *value);

#endif /* !EXPANSION_H */
