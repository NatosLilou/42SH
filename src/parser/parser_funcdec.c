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

static struct token *pop_and_peek(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free_token(tok);
    tok = lexer_peek(lexer);
    return tok;
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
        if (!is_name(tok->value))
        {
            *syntax_error = true;
            free_token(tok);
            goto error;
        }
        ast->name = tok->value;
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

        /*lexer_pop(lexer);
        free_token(tok);
        tok = lexer_peek(lexer);*/
        tok = pop_and_peek(lexer, tok);

        if (!tok)
        {
            goto error;
        }
        if (tok->type != TOKEN_RPAR)
        {
            *syntax_error = true;
            goto error;
        }

        tok = pop_and_peek(lexer, tok);
        /*lexer_pop(lexer);
        free_token(tok);
        tok = lexer_peek(lexer);*/
        while (tok->type == TOKEN_NEWLINE)
        {
            /*lexer_pop(lexer);
            free_token(tok);
            tok = lexer_peek(lexer);*/
            tok = pop_and_peek(lexer, tok);
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
