#include "parser.h"

struct ast_funcdec *parse_funcdec(struct lexer *lexer, bool *syntax_error)
{
    struct ast_funcdec *ast = new_ast_funcdec();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_WORD)
    {
        pop_and_free(lexer, tok);
        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        if (strcmp(tok->value, "(") != 0)
        {
            *syntax_error = true;
            goto error;
        }

        pop_and_free(lexer, tok);
        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        if (strcmp(tok->value, ")") != 0)
        {
            *syntax_error = true;
            goto error;
        }

        pop_and_free(lexer, tok);
        tok = lexer_peek(lexer);
        while (tok->type == TOKEN_NEWLINE)
        {
            lexer_pop(lexer);
            free_token(tok);
            tok = lexer_peek(lexer);
            if (!tok)
            {
                goto error;
            }
        }

        struct ast_shell_command *baby = parse_shell_command(lexer, syntax_error);
        if (baby)
        {
            ast->shell_command = baby;
            return ast;
        }
    }

error:
    free_ast_funcdec(ast);
    return NULL;
}
