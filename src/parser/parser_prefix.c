#include "parser.h"

struct ast_prefix *parse_prefix(struct lexer *lexer)
{
    struct ast_prefix *ast = new_ast_prefix();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_ASSIGNMENT_WORD)
    {
        ast->var = tok->value;
        lexer_pop(lexer);
        free_token(tok);

        return ast;
    }

    struct ast_redir *baby = parse_redir(lexer);
    if (baby)
    {
        ast->redir = baby;
        return ast;
    }

error:
    free_ast_prefix(ast); // Free if fail
    return NULL;
}
