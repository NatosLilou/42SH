//#include <stdio.h>

#include "parser.h"

enum parser_status parse_input(struct ast **res, struct lexer *lexer)
{
    if (parse_list(res /* TODO */, lexer) == PARSER_UNEXPECTED_TOKEN)
    {
        // printf("parse_input: parse_list received unexpected token\n");
        return PARSER_UNEXPECTED_TOKEN;
    }
    if (lexer_peek(lexer).type == TOKEN_NEWLINE)
    {
        // printf("parse_input: lexing newline\n");
        lexer_pop(lexer);
        return PARSER_OK;
    }
    if (lexer_peek(lexer).type == TOKEN_EOF)
    {
        // printf("parse_input: lexing eof\n");
        return PARSER_OK;
    }

    // printf("parse_input: unexpected token\n");
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_list(struct ast **res, struct lexer *lexer)
{
    // printf("parse_list\n");
    return parse_and_or(res /* TODO */, lexer);
}

enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
    // printf("parse_and_or\n");
    return parse_pipeline(res /* TODO */, lexer);
}

enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
    // printf("parse_pipeline\n");
    return parse_command(res /* TODO */, lexer);
}

enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    // printf("parse_command\n");
    return parse_simple_command(res /* TODO */, lexer);
}

enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        // printf("parse_simple_command: lexing word\n");
        struct token tok = lexer_pop(lexer);
        // TODO AST

        while (parse_element(res /* TODO */, lexer) == PARSER_OK)
        {
            // printf("parse_simple_command: parse_list received parser_ok\n");
            //  TODO AST ?
        }

        return PARSER_OK;
    }

    // printf("parse_simple_command: unexpected token\n");
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_element(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        // printf("parse_element: lexing word\n");
        struct token tok = lexer_pop(lexer);
        // TODO AST

        return PARSER_OK;
    }

    return PARSER_UNEXPECTED_TOKEN;
}
