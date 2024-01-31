#include "parser.h"

static void pop_and_free(struct lexer *lexer, struct token *tok)
{
    lexer_pop(lexer);
    free(tok->value);
    free_token(tok);
}

struct ast_else_clause *parse_else_clause(struct lexer *lex, bool *se, int l)
{
    struct ast_else_clause *ast = new_ast_else_clause();
    ast->loop_stage = l;

    struct token *tok = lexer_peek(lex);
    if (!tok)
    {
        *se = true;
        goto error;
    }
    if (tok->type == TOKEN_ELSE)
    {
        pop_and_free(lex, tok);

        struct ast_compound_list *baby = parse_compound_list(lex, se, l);
        if (baby)
        {
            ast->compound_list_elif = baby;
            return ast;
        }
        *se = true;
    }
    else if (tok->type == TOKEN_ELIF)
    {
        pop_and_free(lex, tok);

        struct ast_compound_list *baby2 = parse_compound_list(lex, se, l);
        if (baby2)
        {
            ast->compound_list_elif = baby2;

            tok = lexer_peek(lex);
            if (!tok)
            {
                *se = true;
                goto error;
            }
            if (tok->type == TOKEN_THEN)
            {
                pop_and_free(lex, tok);

                struct ast_compound_list *b3 = parse_compound_list(lex, se, l);
                if (b3)
                {
                    ast->compound_list_then = b3;

                    struct ast_else_clause *b4 = parse_else_clause(lex, se, l);
                    if (b4)
                    {
                        ast->else_clause = b4;
                    }
                    if (*se)
                    {
                        goto error;
                    }

                    return ast;
                }
            }
        }
        *se = true;
    }

error:
    free_ast_else_clause(ast);
    return NULL;
}
