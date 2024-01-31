#include "parser.h"

static bool parse_clanged(struct ast_shell_command *ast, struct lexer *lexer,
                          bool *syntax_error, int loop_stage)
{
    struct ast_rule_if *baby1 = parse_rule_if(lexer, syntax_error, loop_stage);
    if (baby1)
    {
        ast->rule_if = baby1;
        return true;
    }
    if (*syntax_error)
    {
        return false;
    }

    struct ast_rule_while *baby2 =
        parse_rule_while(lexer, syntax_error, loop_stage);
    if (baby2)
    {
        ast->rule_while = baby2;
        return true;
    }
    if (*syntax_error)
    {
        return false;
    }

    struct ast_rule_until *baby3 =
        parse_rule_until(lexer, syntax_error, loop_stage);
    if (baby3)
    {
        ast->rule_until = baby3;
        return true;
    }
    if (*syntax_error)
    {
        return false;
    }

    struct ast_rule_for *baby4 =
        parse_rule_for(lexer, syntax_error, loop_stage);
    if (baby4)
    {
        ast->rule_for = baby4;
        return true;
    }
    if (*syntax_error)
    {
        return false;
    }

    struct ast_rule_case *baby5 =
        parse_rule_case(lexer, syntax_error, loop_stage);
    if (baby5)
    {
        ast->rule_case = baby5;
        return true;
    }

    return false;
}

struct ast_shell_command *
parse_shell_command(struct lexer *lexer, bool *syntax_error, int loop_stage)
{
    struct ast_shell_command *ast = new_ast_shell_command();
    ast->loop_stage = loop_stage;

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_LBRACE || tok->type == TOKEN_LPAR)
    {
        ast->sub = tok->type == TOKEN_LPAR;
        lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        struct ast_compound_list *baby =
            parse_compound_list(lexer, syntax_error, loop_stage);
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
        if ((tok->type == TOKEN_RBRACE && !ast->sub)
            || (ast->sub && tok->type == TOKEN_RPAR))
        {
            lexer_pop(lexer);
            free(tok->value);
            free_token(tok);
            return ast;
        }
        *syntax_error = true;
        goto error;
    }

    if (parse_clanged(ast, lexer, syntax_error, loop_stage))
    {
        return ast;
    }

error:
    free_ast_shell_command(ast); // Free if fail
    return NULL;
}
