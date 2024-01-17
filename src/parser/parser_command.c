#include "parser.h"

struct ast_command *parse_command(struct lexer *lexer)
{
    struct ast_command *ast = new_ast_command();

    struct ast_simple_command *baby = parse_simple_command(lexer);
    if (baby)
    {
        ast->simple_command = baby;
        return ast;
    }

    struct ast_shell_command *baby2 = parse_shell_command(lexer);
    if (baby2)
    {
        ast->shell_command = baby2;
        
        struct ast_redir *baby3 = parse_redir(lexer);
        while (baby3)
        {
            add_ast_command(ast, baby3);
            baby3 = parse_redir(lexer);
        }

        return ast;
    }

    free_ast_command(ast); // Free if fail

    return NULL;
}
