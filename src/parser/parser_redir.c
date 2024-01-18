#include "parser.h"

struct ast_redir *parse_redir(struct lexer *lexer)
{
    struct ast_redir *ast = new_ast_redir();

    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_IO_NUMBER)
    {
        lexer_pop(lexer);
        ast->ionumber = atoi(tok->value);
        free_token(tok);
    }

    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_LESS || tok->type == TOKEN_GREAT
        || tok->type == TOKEN_DGREAT || tok->type == TOKEN_LESSAND
        || tok->type == TOKEN_GREATAND || tok->type == TOKEN_CLOBBER
        || tok->type == TOKEN_LESSGREAT)
    {
        ast->redir = tok->type;
        lexer_pop(lexer);
        free_token(tok);

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_WORD || tok->type == TOKEN_IF
            || tok->type == TOKEN_THEN || tok->type == TOKEN_ELSE
            || tok->type == TOKEN_ELIF || tok->type == TOKEN_FI
            || tok->type == TOKEN_DO || tok->type == TOKEN_DONE
            || tok->type == TOKEN_WHILE || tok->type == TOKEN_UNTIL
            || tok->type == TOKEN_FOR || tok->type == TOKEN_BANG
            || tok->type == TOKEN_IN)
        {
            ast->dest = tok->value;
            lexer_pop(lexer);
            free_token(tok);
            return ast;
        }
    }

    free_ast_redir(ast);
    return NULL;
}
