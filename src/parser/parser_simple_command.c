#include "parser.h"

struct ast_simple_command *parse_simple_command(struct lexer *lexer)
{
    struct ast_simple_command *ast = new_ast_simple_command();

    struct ast_prefix *baby = parse_prefix(lexer);
    while (baby)
    {
        add_ast_simple_command_pref(ast, baby);
        baby = parse_prefix(lexer);
    }

    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_WORD)
    {
        add_ast_simple_command_cmd(ast, tok->value);
        lexer_pop(lexer);
        free_token(tok);

        bool elt = parse_element(ast, lexer);
        while (elt)
        {
            elt = parse_element(ast, lexer);
        }

        return ast;
    }
    else if (!ast->prefix)
    {
        return NULL;
    }
    else
    {
        return ast;
    }
}
