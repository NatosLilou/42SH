#include "parser.h"

struct ast_compound_list *parse_compound_list(struct lexer *lexer)
{
    struct ast_compound_list *ast = new_ast_compound_list();

    while (lexer_peek(lexer)->type == TOKEN_NEWLINE)
    {
        struct token *tok = lexer_pop(lexer);
        free_token(tok);
    }

    struct ast_and_or *baby = parse_and_or(lexer);
    if (!baby)
    {
        free_ast_compound_list(ast);
        return NULL;
    }
    add_ast_compound_list(ast, baby);

    struct token *tok2 = lexer_peek(lexer);
    while (tok2->type == TOKEN_SEMI || tok2->type == TOKEN_NEWLINE)
    {
        tok2 = lexer_pop(lexer);
        free_token(tok2);

        while (lexer_peek(lexer)->type == TOKEN_NEWLINE)
        {
            tok2 = lexer_pop(lexer);
            free_token(tok2);
        }

        tok2 = lexer_peek(lexer);
        if (tok2->type == TOKEN_THEN || tok2->type == TOKEN_ELIF
            || tok2->type == TOKEN_ELSE || tok2->type == TOKEN_FI
            || tok2->type == TOKEN_DO || tok2->type == TOKEN_DONE)
        {
            return ast;
        }
        struct ast_and_or *baby2 = parse_and_or(lexer);
        if (!baby2)
        {
            return ast;
        }
        add_ast_compound_list(ast, baby2);
    }

    if (lexer_peek(lexer)->type == TOKEN_SEMI)
    {
        struct token *tok = lexer_pop(lexer);
        free_token(tok);
    }

    while (lexer_peek(lexer)->type == TOKEN_NEWLINE)
    {
        struct token *tok = lexer_pop(lexer);
        free_token(tok);
    }

    return ast;
}
