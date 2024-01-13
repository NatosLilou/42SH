#include "parser.h"

struct ast_shell_command *parse_shell_command(struct lexer *lexer)
{
    struct ast_shell_command *ast = new_ast_shell_command();

    struct ast_rule_if *baby = parse_rule_if(lexer);
    if (baby)
    {
        ast->rule_if = baby;
        return ast;
    }

    free_ast_shell_command(ast); // Free if fail

    return NULL;
}
