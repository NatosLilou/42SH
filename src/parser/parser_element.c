#include "parser.h"

bool parse_element(struct ast_simple_command *ast, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_WORD)
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
