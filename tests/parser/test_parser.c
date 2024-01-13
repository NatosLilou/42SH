#include <parser.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);

    struct ast *ast;
    enum parser_status status = parse_input(&ast, lexer);
    if (status != PARSER_OK)
    {
        lexer_free(lexer);
        return 1;
    }

    // print_ast(ast);
    // printf("\n");

    // ast_free(ast);
    lexer_free(lexer);

    return 0;
}
