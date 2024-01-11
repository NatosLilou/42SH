#include <stdio.h>

#include "../../lexer/lexer.h"
#include "../../parser/parser.h"
#include "../ast.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    struct lexer *lexer = lexer_new(argv[1]);

    struct ast_input *ast = parse_input(lexer);
    if (!ast)
    {
        return 1;
    }

    print_ast_input(ast);

    return 0;
}
