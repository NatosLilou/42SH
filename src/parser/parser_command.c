#include "parser.h"

struct ast_command *parse_command(struct lexer *lexer, bool *syntax_error,
                                  int loop_stage)
{
    struct ast_command *ast = new_ast_command();
    ast->loop_stage = loop_stage;

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        goto error;
    }
    if (tok->type == TOKEN_WORD)
    {
        struct token *next = lexer_peek_ahead(lexer);
        if (next->type == TOKEN_LPAR)
        {
            struct ast_funcdec *baby =
                parse_ast_funcdec(lexer, syntax_error, loop_stage);
            if (baby)
            {
                ast->funcdec = baby;
                return ast;
            }
            if (*syntax_error)
            {
                goto error;
            }
        }
    }
    struct ast_simple_command *baby =
        parse_simple_command(lexer, syntax_error, loop_stage);
    if (baby)
    {
        ast->simple_command = baby;
        return ast;
    }
    if (*syntax_error)
    {
        goto error;
    }

    struct ast_shell_command *baby2 =
        parse_shell_command(lexer, syntax_error, loop_stage);
    if (baby2)
    {
        ast->shell_command = baby2;

        struct ast_redir *baby3 = parse_redir(lexer, syntax_error, loop_stage);
        while (baby3)
        {
            add_ast_command(ast, baby3);
            baby3 = parse_redir(lexer, syntax_error, loop_stage);
        }
        if (*syntax_error)
        {
            goto error;
        }

        return ast;
    }

error:
    free_ast_command(ast); // Free if fail
    return NULL;
}
