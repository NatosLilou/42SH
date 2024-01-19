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

    struct ast_rule_while *baby2 = parse_rule_while(lexer);
    if (baby2)
    {
        ast->rule_while = baby2;
        return ast;
    }

    struct ast_rule_until *baby3 = parse_rule_until(lexer);
    if (baby3)
    {
        ast->rule_until = baby3;
        return ast;
    }

    struct ast_rule_for *baby4 = parse_rule_for(lexer);
    if (baby4)
    {
        ast->rule_for = baby4;
        return ast;
    }

    free_ast_shell_command(ast); // Free if fail
    return NULL;
}
