#include "parser.h"

static bool is_reserved(enum token_type type)
{
    return (type == TOKEN_IF || type == TOKEN_THEN || type == TOKEN_ELIF
            || type == TOKEN_ELSE || type == TOKEN_FI || type == TOKEN_WHILE
            || type == TOKEN_UNTIL || type == TOKEN_DO || type == TOKEN_DONE
            || type == TOKEN_FOR || type == TOKEN_IN || type == TOKEN_BANG
            || type == TOKEN_LBRACE || type == TOKEN_RBRACE
            || type == TOKEN_CASE || type == TOKEN_ESAC);
}

int parse_element(struct ast_simple_command *ast, struct lexer *lexer,
                  bool *syntax_error, int loop_stage)
{
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        return -1;
    }
    if (tok->type == TOKEN_WORD || is_reserved(tok->type))
    {
        add_ast_simple_command_cmd(ast, tok->value);
        lexer_pop(lexer);
        free_token(tok);
        return 1;
    }

    struct ast_redir *redir = parse_redir(lexer, syntax_error, loop_stage);
    if (redir)
    {
        add_ast_simple_command_redir(ast, redir);
        return 1;
    }

    return 0;
}
