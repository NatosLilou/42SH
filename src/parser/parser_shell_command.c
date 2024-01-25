#include "parser.h"

static struct ast_shell_command *parse_clanged(struct ast_shell_command *ast,
        struct lexer *lexer, bool *syntax_error)
{
    struct ast_rule_if *baby1 = parse_rule_if(lexer, syntax_error);
    if (baby1)
    {
        ast->rule_if = baby1;
        return ast;
    }
    if (*syntax_error)
    {
        return NULL;
    }

    struct ast_rule_while *baby2 = parse_rule_while(lexer, syntax_error);
    if (baby2)
    {
        ast->rule_while = baby2;
        return ast;
    }
    if (*syntax_error)
    {
        return NULL;
    }

    struct ast_rule_until *baby3 = parse_rule_until(lexer, syntax_error);
    if (baby3)
    {
        ast->rule_until = baby3;
        return ast;
    }
    if (*syntax_error)
    {
        return NULL;
    }

    struct ast_rule_for *baby4 = parse_rule_for(lexer, syntax_error);
    if (baby4)
    {
        ast->rule_for = baby4;
        return ast;
    }

    return NULL;

}

struct ast_shell_command *parse_shell_command(struct lexer *lexer,
                                              bool *syntax_error)
{
    struct ast_shell_command *ast = new_ast_shell_command();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if ((tok->type == TOKEN_WORD && strcmp(tok->value, "{") == 0)
            || tok->type == TOKEN_LPAR)
    {
        ast->sub = tok->type == TOKEN_LPAR;
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby =
            parse_compound_list(lexer, syntax_error);
        if (!baby)
        {
            *syntax_error = true;
            goto error;
        }
        ast->compound_list = baby;

        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        if ((tok->type == TOKEN_WORD && strcmp(tok->value, "}") == 0 
                    && !ast->sub) || (ast->sub && tok->type == TOKEN_RPAR))
        {
            lexer_pop(lexer);
            free(tok->value);
            free_token(tok);
            return ast;
        }
        *syntax_error = true;
        goto error;
    }

    ast = parse_clanged(ast, lexer, syntax_error);
    if (ast)
    {
        return ast;
    }

error:
    free_ast_shell_command(ast); // Free if fail
    return NULL;
}
