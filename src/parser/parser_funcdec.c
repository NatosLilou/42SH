#include "parser.h"

static bool is_name(char *value)
{
    size_t i = 0;

    if (value[i] >= '0' && value[i] <= '9')
    {
        return false;
    }

    while (value[i] != '\0')
    {
        if (value[i] == '_' || (value[i] >= '0' && value[i] <= '9')
            || (value[i] >= 'a' && value[i] <= 'z')
            || (value[i] >= 'A' && value[i] <= 'Z'))
        {
            i++;
        }
        else
        {
            free(value);
            return false;
        }
    }

    return true;
}

struct ast_funcdec *parse_ast_funcdec(struct lexer *lexer, bool *syntax_error)
{
    struct ast_funcdec *ast = new_ast_funcdec();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_WORD)
    {
        lexer_pop(lexer);
        ast->name = is_name(tok->value) ? tok->value : NULL;
        free_token(tok);
        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        if (tok->type != TOKEN_LPAR)
        {
            *syntax_error = true;
            goto error;
        }

        lexer_pop(lexer);
        free_token(tok);
        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        if (tok->type != TOKEN_RPAR)
        {
            *syntax_error = true;
            goto error;
        }

        lexer_pop(lexer);
        free_token(tok);
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

        struct ast_shell_command *baby =
            parse_shell_command(lexer, syntax_error);
        if (baby)
        {
            ast->shell_command = baby;
            return ast;
        }
        *syntax_error = true;
    }

error:
    free_ast_funcdec(ast);
    return NULL;
}
