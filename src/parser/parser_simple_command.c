#include "parser.h"

static bool is_reserved(enum token_type type)
{
    return (type == TOKEN_IF || type == TOKEN_THEN || type == TOKEN_ELIF
            || type == TOKEN_ELSE || type == TOKEN_FI || type == TOKEN_WHILE
            || type == TOKEN_UNTIL || type == TOKEN_DO || type == TOKEN_DONE
            || type == TOKEN_FOR || type == TOKEN_IN || type == TOKEN_BANG);
}

struct ast_simple_command *parse_simple_command(struct lexer *lexer)
{
    struct ast_simple_command *ast = new_ast_simple_command();

    bool prefix = false; // To know if WORD can be RESERVED_WORD

    struct ast_prefix *baby = parse_prefix(lexer);
    while (baby)
    {
        prefix = true;
        add_ast_simple_command_pref(ast, baby);
        baby = parse_prefix(lexer);
    }

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_WORD || (prefix && (is_reserved(tok->type))))
    {
        add_ast_simple_command_cmd(ast, tok->value);
        lexer_pop(lexer);
        free_token(tok);

        int elt = parse_element(ast, lexer);
        while (elt == 1)
        {
            elt = parse_element(ast, lexer);
        }
        if (elt == -1)
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
