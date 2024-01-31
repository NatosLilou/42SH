#include "parser.h"

struct ast_list *parse_list(struct lexer *lexer, bool *syntax_error,
                            int loop_stage)
{
    struct ast_list *ast = new_ast_list();
    ast->loop_stage = loop_stage;

    struct ast_and_or *baby = parse_and_or(lexer, syntax_error, loop_stage);
    if (!baby)
    {
        goto error;
    }

    add_ast_list(ast, baby);

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    while (tok->type == TOKEN_SEMI)
    {
        lexer_pop(lexer);
        free_token(tok);

        struct ast_and_or *baby2 =
            parse_and_or(lexer, syntax_error, loop_stage);
        if (!baby2)
        {
            break;
        }
        add_ast_list(ast, baby2);

        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
    }
    if (*syntax_error)
    {
        goto error;
    }

    return ast;

error:
    free_ast_list(ast);
    return NULL;
}
