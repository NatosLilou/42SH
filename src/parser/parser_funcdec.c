#include "parser.h"

extern struct assigned_var *assigned;

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

static struct token *pop_and_peek(struct lexer *lexer, struct token *tok,
                                  bool *syntax_error)
{
    lexer_pop(lexer);
    free_token(tok);
    tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
    }
    return tok;
}

static bool newline_loop(struct lexer *lexer, struct token *tok,
                         bool *syntax_error)
{
    while (tok->type == TOKEN_NEWLINE)
    {
        tok = pop_and_peek(lexer, tok, syntax_error);
        if (!tok)
        {
            return false;
        }
    }
    return true;
}

struct ast_funcdec *parse_ast_funcdec(struct lexer *lexer, bool *syntax_error,
                                      int loop_stage)
{
    struct ast_funcdec *ast = new_ast_funcdec();
    ast->loop_stage = loop_stage;

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
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
        if (!tok || tok->type != TOKEN_LPAR)
        {
            *syntax_error = true;
            goto error;
        }

        tok = pop_and_peek(lexer, tok, syntax_error);
        if (!tok || tok->type != TOKEN_RPAR)
        {
            *syntax_error = true;
            goto error;
        }

        tok = pop_and_peek(lexer, tok, syntax_error);
        if (!tok)
        {
            *syntax_error = true;
            goto error;
        }

        if (!newline_loop(lexer, tok, syntax_error))
        {
            goto error;
        }

        struct ast_shell_command *baby =
            parse_shell_command(lexer, syntax_error, loop_stage);
        if (baby)
        {
            assigned->shell_commands[assigned->pos_shell] = baby;
            assigned->pos_shell++;
            ast->shell_command = baby;
            return ast;
        }
        *syntax_error = true;
    }

error:
    free_ast_funcdec(ast);
    return NULL;
}
