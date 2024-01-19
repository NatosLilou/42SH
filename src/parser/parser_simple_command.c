#include "parser.h"

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
    if (tok->type == TOKEN_WORD
        || (prefix
            && (tok->type == TOKEN_IF || tok->type == TOKEN_THEN
                || tok->type == TOKEN_ELIF || tok->type == TOKEN_ELSE
                || tok->type == TOKEN_FI || tok->type == TOKEN_WHILE
                || tok->type == TOKEN_UNTIL || tok->type == TOKEN_DO
                || tok->type == TOKEN_DONE || tok->type == TOKEN_FOR
                || tok->type == TOKEN_IN || tok->type == TOKEN_BANG)))
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
    else if (ast->pos_pref == 0)
    {
        free_ast_simple_command(ast);
        return NULL;
    }
    else
    {
        return ast;
    }
}
