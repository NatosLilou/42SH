//#include <stdio.h>

#include "parser.h"

struct ast_input *parse_input(struct lexer *lexer)
{
    struct ast_input *ast = new_ast_input();

    if (!struct ast_list *baby = parse_list(lexer))
    {
        // printf("parse_input: parse_list received unexpected token\n");
        return NULL;
    }

    ast->value = baby;

    if (lexer_peek(lexer).type == TOKEN_NEWLINE)
    {
        // printf("parse_input: lexing newline\n");
        lexer_pop(lexer);
        return ast;
    }
    if (lexer_peek(lexer).type == TOKEN_EOF)
    {
        // printf("parse_input: lexing eof\n");
        return ast;
    }

    // printf("parse_input: unexpected token\n");
    return NULL;
}

struct ast_list *parse_list(struct lexer *lexer)
{
    // printf("parse_list\n");
    struct ast_list *ast = new_ast_list();

    if (struct ast_and_or *baby = parse_and_or(lexer))
    {
        ast->value = baby;
        return ast;
    }

    return NULL;
}

struct ast_and_or *parse_and_or(struct lexer *lexer)
{
    // printf("parse_and_or\n");
    struct ast_and_or *ast = new_ast_and_or();

    if (struct ast_pipeline *baby = parse_pipeline(lexer))
    {
        ast->value = baby;
        return ast;
    }

    return NULL;
}

struct ast_pipeline *parse_pipeline(struct lexer *lexer)
{
    // printf("parse_pipeline\n");
    struct ast_pipeline *ast = new_ast_pipeline();

    if (struct ast_command *baby = parse_command(lexer))
    {
        ast->value = baby;
        return ast;
    }

    return NULL;
}

struct ast_command *parse_command(struct lexer *lexer)
{
    // printf("parse_command\n");
    struct ast_command *ast = new_ast_command();

    if (struct ast_simple_command *baby = parse_simple_command(lexer))
    {
        ast->value = baby;
        return ast;
    }

    return NULL;
}

struct ast_simple_command *parse_simple_command(struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        // printf("parse_simple_command: lexing word\n");
        struct token tok = lexer_pop(lexer);
        struct ast_simple_command *ast = new_ast_simple_command(tok->value);

        while (char *element = parse_element(lexer))
        {
            // printf("parse_simple_command: parse_list received parser_ok\n");
            //  TODO ADD ELEMENT TO AST
        }

        return ast;
    }

    // printf("parse_simple_command: unexpected token\n");
    return NULL;
}

char *parse_element(struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        // printf("parse_element: lexing word\n");
        struct token tok = lexer_pop(lexer);
        return tok->value;
    }

    return NULL;
}
