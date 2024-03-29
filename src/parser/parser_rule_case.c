#include "parser.h"

static bool is_reserved(enum token_type type)
{
    return (type == TOKEN_IF || type == TOKEN_ELIF || type == TOKEN_ELSE
            || type == TOKEN_FI || type == TOKEN_WHILE || type == TOKEN_UNTIL
            || type == TOKEN_DO || type == TOKEN_DONE || type == TOKEN_FOR
            || type == TOKEN_IN || type == TOKEN_BANG || type == TOKEN_CASE
            || type == TOKEN_ESAC);
}

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

static struct token *pop_and_peek(struct lexer *lexer, struct token *tok,
                                  bool *syntax_error)
{
    lexer_pop(lexer);
    free_token(tok);
    tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
    }
    return tok;
}

static bool newline_loop(struct lexer *lexer, bool *syntax_error)
{
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        return false;
    }
    while (tok->type == TOKEN_NEWLINE)
    {
        tok = pop_and_peek(lexer, tok, syntax_error);
        if (!tok)
        {
            return false;
        }
    }
    return true;
}

static bool parsing_case_clause(struct ast_rule_case *ast, struct lexer *lexer,
                                bool *syntax_error)
{
    struct ast_case_clause *baby =
        parse_case_clause(lexer, syntax_error, ast->loop_stage);
    if (baby)
    {
        ast->case_clause = baby;
    }
    if (*syntax_error)
    {
        return false;
    }
    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        return false;
    }
    if (tok->type == TOKEN_ESAC)
    {
        pop_and_free(lexer, tok);
        return true;
    }

    *syntax_error = true;
    return false;
}

struct ast_rule_case *parse_rule_case(struct lexer *lexer, bool *syntax_error,
                                      int loop_stage)
{
    struct ast_rule_case *ast = new_ast_rule_case();
    ast->loop_stage = loop_stage;

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        *syntax_error = true;
        goto error;
    }
    if (tok->type == TOKEN_CASE)
    {
        pop_and_free(lexer, tok);

        tok = lexer_peek(lexer);
        if (!tok)
        {
            *syntax_error = true;
            goto error;
        }
        if (tok->type == TOKEN_WORD || is_reserved(tok->type))
        {
            ast->word = tok->value;
            lexer_pop(lexer);
            free_token(tok);

            if (!newline_loop(lexer, syntax_error))
            {
                goto error;
            }

            tok = lexer_peek(lexer);
            if (!tok)
            {
                *syntax_error = true;
                goto error;
            }

            if (tok->type == TOKEN_IN)
            {
                pop_and_free(lexer, tok);

                if (!newline_loop(lexer, syntax_error))
                {
                    goto error;
                }

                if (parsing_case_clause(ast, lexer, syntax_error))
                {
                    return ast;
                }
            }
        }

        *syntax_error = true;
    }

error:
    free_ast_rule_case(ast);
    return NULL;
}
