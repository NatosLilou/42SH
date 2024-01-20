#include "parser.h"

static bool is_assignment_word(char *value)
{
    size_t i = 0;

    if (value[i] >= '0' && value[i] <= '9')
    {
        return false;
    }

    while (value[i] != '=' && value[i] != '\0')
    {
        if (value[i] == '_' || (value[i] >= '0' && value[i] <= '9')
            || (value[i] >= 'a' && value[i] <= 'z')
            || (value[i] >= 'A' && value[i] <= 'Z'))
        {
            i++;
        }
        else
        {
            return false;
        }
    }

    if (value[i] == '=')
    {
        return true;
    }
    return false;
}

struct ast_prefix *parse_prefix(struct lexer *lexer)
{
    struct ast_prefix *ast = new_ast_prefix();

    struct token *tok = lexer_peek(lexer);
    if (!tok)
    {
        goto error;
    }
    if (tok->type == TOKEN_WORD && is_assignment_word(tok->value))
    {
        bool equal = false;

        char *name = calloc(strlen(tok->value), 1);
        char *value = calloc(strlen(tok->value), 1);

        size_t pos_res = 0;
        size_t pos_tok = 0;

        while (tok->value[pos_tok] != '\0')
        {
            if (!equal && tok->value[pos_tok] == '=')
            {
                equal = true;
                pos_res = 0;
            }
            else if (equal)
            {
                value[pos_res] = tok->value[pos_tok];
                pos_res++;
            }
            else
            {
                name[pos_res] = tok->value[pos_tok];
                pos_res++;
            }
            pos_tok++;
        }

        free(tok->value);

        ast->name = name;
        ast->value = value;
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
