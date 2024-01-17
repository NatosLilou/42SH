#include "parser.h"

bool parse_element(struct ast_simple_command *ast, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_WORD || tok->type == TOKEN_IF
        || tok->type == TOKEN_ELIF || tok->type == TOKEN_ELSE
        || tok->type == TOKEN_FI || tok->type == TOKEN_WHILE
        || tok->type == TOKEN_UNTIL || tok->type == TOKEN_DO
        || tok->type == TOKEN_DONE || tok->type == TOKEN_FOR
        || tok->type == TOKEN_IN)
    {
        add_ast_simple_command_cmd(ast, tok->value);
        lexer_pop(lexer);
        free_token(tok);
        return true;
    }

    struct ast_redir *redir = parse_redir(lexer);
    if (redir)
    {
        add_ast_simple_command_redir(ast, redir);
        return true;
    }

    return false;
}
