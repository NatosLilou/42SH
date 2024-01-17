#include "parser.h"

static bool is_reserved(enum token_type type)
{
    return (type == TOKEN_IF || type == TOKEN_ELIF || type == TOKEN_ELSE
            || type == TOKEN_FI || type == TOKEN_WHILE || type == TOKEN_UNTIL
            || type == TOKEN_DO || type == TOKEN_DONE || type == TOKEN_FOR
            || type == TOKEN_IN);
}

static int parse_rule_for_group(struct ast_rule_for *ast, struct lexer *lexer)
{
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_SEMI)
    {
        lexer_pop(lexer);
        free_token(tok);
    }
    else if (tok->type == TOKEN_NEWLINE)
    {
        while (tok->type == TOKEN_NEWLINE)
        {
            lexer_pop(lexer);
            free_token(tok);
            tok = lexer_peek(lexer);
        }

        if (tok->type == TOKEN_IN)
        {
            lexer_pop(lexer);
            free(tok->value);
            free_token(tok);

            tok = lexer_peek(lexer);
            while (tok->type == TOKEN_WORD || is_reserved(tok->type))
            {
                add_ast_rule_for(ast, tok->value);
                lexer_pop(lexer);
                free_token(tok);
                tok = lexer_peek(lexer);
            }

            if (tok->type == TOKEN_SEMI || tok->type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
                free_token(tok);
            }
            else
            {
                return 0;
            }
        }
    }
    return 1;
}

struct ast_rule_for *parse_rule_for(struct lexer *lexer)
{
    struct ast_rule_for *ast = new_ast_rule_for();

    if (lexer_peek(lexer)->type == TOKEN_FOR)
    {
        struct token *tok = lexer_pop(lexer);
        free(tok->value);
        free_token(tok);

        tok = lexer_peek(lexer);
        if (tok->type == TOKEN_WORD || is_reserved(tok->type))
        {
            add_ast_rule_for(ast, tok->value);
            lexer_pop(lexer);
            free_token(tok);

            if (!parse_rule_for_group(ast, lexer))
            {
                free_ast_rule_for(ast);
                return NULL;
            }

            tok = lexer_peek(lexer);
            while (tok->type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
                free_token(tok);
                tok = lexer_peek(lexer);
            }

            tok = lexer_peek(lexer);
            if (tok->type == TOKEN_DO)
            {
                lexer_pop(lexer);
                free(tok->value);
                free_token(tok);

                struct ast_compound_list *baby = parse_compound_list(lexer);
                if (baby)
                {
                    ast->compound_list = baby;

                    tok = lexer_peek(lexer);
                    if (tok->type == TOKEN_DONE)
                    {
                        lexer_pop(lexer);
                        free(tok->value);
                        free_token(tok);

                        return ast;
                    }
                }
            }
        }
    }

    free_ast_rule_for(ast);
    return NULL;
}
