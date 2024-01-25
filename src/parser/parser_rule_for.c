#include "parser.h"

static bool is_reserved(enum token_type type)
{
    return (type == TOKEN_IF || type == TOKEN_ELIF || type == TOKEN_ELSE
            || type == TOKEN_FI || type == TOKEN_WHILE || type == TOKEN_UNTIL
            || type == TOKEN_DO || type == TOKEN_DONE || type == TOKEN_FOR
            || type == TOKEN_IN || type == TOKEN_BANG);
}

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

static int parse_rule_for_group(struct ast_rule_for *ast, struct lexer *lexer,
                                bool *syntax_error)
{
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        return 0;
    }
    if (tok->type == TOKEN_SEMI)
    {
        lexer_pop(lexer);
        free_token(tok);
    }
    else if (tok->type == TOKEN_NEWLINE || tok->type == TOKEN_IN)
    {
        while (tok->type == TOKEN_NEWLINE)
        {
            lexer_pop(lexer);
            free_token(tok);
            tok = lexer_peek(lexer);
            if (!tok)
            {
                return 0;
            }
        }

        if (tok->type == TOKEN_IN)
        {
            pop_and_free(lexer, tok);

            tok = lexer_peek(lexer);
            if (!tok)
            {
                return 0;
            }
            while (tok->type == TOKEN_WORD || is_reserved(tok->type))
            {
                add_ast_rule_for(ast, tok->value);
                lexer_pop(lexer);
                free_token(tok);
                tok = lexer_peek(lexer);
                if (!tok)
                {
                    return 0;
                }
            }

            if (tok->type == TOKEN_SEMI || tok->type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
                free_token(tok);
            }
            else
            {
                *syntax_error = true;
                return 0;
            }
        }
        else
        {
            *syntax_error = true;
            return 0;
        }
    }

    return 1;
}

struct ast_rule_for *parse_rule_for(struct lexer *lexer, bool *syntax_error)
{
    struct ast_rule_for *ast = new_ast_rule_for();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_FOR)
    {
        pop_and_free(lexer, tok);

        tok = lexer_peek(lexer);
        if (!tok)
        {
            goto error;
        }
        if (tok->type == TOKEN_WORD || is_reserved(tok->type))
        {
            add_ast_rule_for(ast, tok->value);
            lexer_pop(lexer);
            free_token(tok);

            if (!parse_rule_for_group(ast, lexer, syntax_error))
            {
                goto error;
            }

            tok = lexer_peek(lexer);
            if (!tok)
            {
                goto error;
            }
            while (tok->type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
                free_token(tok);
                tok = lexer_peek(lexer);
                if (!tok)
                {
                    goto error;
                }
            }

            if (tok->type == TOKEN_DO)
            {
                pop_and_free(lexer, tok);

                struct ast_compound_list *baby =
                    parse_compound_list(lexer, syntax_error);
                if (baby)
                {
                    ast->compound_list = baby;

                    tok = lexer_peek(lexer);
                    if (!tok)
                    {
                        goto error;
                    }
                    if (tok->type == TOKEN_DONE)
                    {
                        pop_and_free(lexer, tok);

                        return ast;
                    }
                }
            }
        }

        *syntax_error = true;
    }

error:
    free_ast_rule_for(ast);
    return NULL;
}
