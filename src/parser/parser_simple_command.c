#include "parser.h"

static bool is_reserved(enum token_type type)
{
    return (type == TOKEN_IF || type == TOKEN_THEN || type == TOKEN_ELIF
            || type == TOKEN_ELSE || type == TOKEN_FI || type == TOKEN_WHILE
            || type == TOKEN_UNTIL || type == TOKEN_DO || type == TOKEN_DONE
            || type == TOKEN_FOR || type == TOKEN_IN || type == TOKEN_BANG
            || type == TOKEN_CASE || type == TOKEN_ESAC);
}

struct ast_simple_command *
parse_simple_command(struct lexer *lexer, bool *syntax_error, int loop_stage)
{
    struct ast_simple_command *ast = new_ast_simple_command();
    ast->loop_stage = loop_stage;

    bool prefix = false; // To know if WORD can be RESERVED_WORD

    struct ast_prefix *baby = parse_prefix(lexer, syntax_error, loop_stage);
    while (baby)
    {
        prefix = true;
        add_ast_simple_command_pref(ast, baby);
        baby = parse_prefix(lexer, syntax_error, loop_stage);
    }
    if (*syntax_error)
    {
        goto error;
    }

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        goto error;
    }
    if ((tok->type == TOKEN_WORD) || (prefix && (is_reserved(tok->type))))
    {
        add_ast_simple_command_cmd(ast, tok->value);
        lexer_pop(lexer);
        free_token(tok);

        int elt = parse_element(ast, lexer, syntax_error, loop_stage);
        while (elt == 1)
        {
            elt = parse_element(ast, lexer, syntax_error, loop_stage);
        }
        if (elt == -1 || *syntax_error)
        {
            goto error;
        }

        return ast;
    }
    else if (ast->pos_pref > 0)
    {
        return ast;
    }

error:
    free_ast_simple_command(ast);
    return NULL;
}
