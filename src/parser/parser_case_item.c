#include "parser.h"

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

static struct token *pop_and_peek(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free_token(tok);
    tok = lexer_peek(lexer);
    return tok;
}

struct ast_case_item *parse_case_item(struct lexer *lexer, bool *syntax_error, int loop_stage)
{
    struct ast_case_item *ast = new_ast_case_item();
    ast->loop_stage = loop_stage;
    bool lpar = false;
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_LPAR)
    {
        lpar = true;
        pop_and_free(lexer, tok);
    }
    tok = lexer_peek(lexer);
    if (tok->type == TOKEN_WORD)
    {
        add_ast_case_item(ast, tok->value);

        lexer_pop(lexer);
        free_token(tok);

        tok = lexer_peek(lexer);
        while (tok->type == TOKEN_PIPE) // { '|' WORD }
        {
            tok = pop_and_peek(lexer, tok);

            if (tok->type == TOKEN_WORD)
            {
                add_ast_case_item(ast, tok->value);
                lexer_pop(lexer);
                free_token(tok);
            }
            else
            {
                *syntax_error = true;
                goto error;
            }

            tok = lexer_peek(lexer);
        }

        if (tok->type != TOKEN_RPAR)
        {
            *syntax_error = true;
            goto error;
        }

        lexer_pop(lexer);
        free_token(tok);
        tok = lexer_peek(lexer);

        while (tok->type == TOKEN_NEWLINE)
        {
            tok = pop_and_peek(lexer, tok);
        }

        struct ast_compound_list *baby =
            parse_compound_list(lexer, syntax_error, loop_stage);
        if (baby)
        {
            ast->compound_list = baby;
        }
        if (*syntax_error)
        {
            goto error;
        }

        return ast;
    }

    if (lpar)
    {
        *syntax_error = true;
    }

error:
    free_ast_case_item(ast);
    return NULL;
}
