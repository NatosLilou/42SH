#include "parser.h"

struct ast_pipeline *parse_pipeline(struct lexer *lexer)
{
    struct ast_pipeline *ast = new_ast_pipeline();
    
    struct token *tok = lexer_peek(lexer);
    if (tok->type == TOKEN_NEG)
    {
        ast->negation = true;
        lexer_pop(lexer);
        free_token(tok);
    }
    
    struct ast_command *baby = parse_command(lexer);
    if (baby)
    {
        add_ast_pipeline(ast, baby);
        
        tok = lexer_peek(lexer);
        while (tok->type == TOKEN_PIPE)
        {
            lexer_pop(lexer);
            free_token(tok);

            tok = lexer_peek(lexer);
            while (tok->type == TOKEN_NEWLINE)
            {
                 lexer_pop(lexer);
                 free_token(tok);
                 tok = lexer_peek(lexer);
            }

            baby = parse_command(lexer);
            if (baby)
            {
                add_ast_pipeline(ast, baby);
            }
            else
            {
                free_ast_pipeline(ast);
                return NULL;
            }
        }

        return ast;
    }

    free_ast_pipeline(ast);
    return NULL;
}
