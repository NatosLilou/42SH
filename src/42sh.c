#include <eval.h>
#include <io_back_end.h>
#include <parser.h>

int main(int argc, char *argv[])
{
    char *input = io_back_end(argc, argv); // TODO INPUT TO FREE

    struct lexer *lexer = new_lexer(input);

    struct ast_input *ast = parse_input(lexer);

    int res = 1;
    while (!ast->eof)
    {
        res = eval_input(ast);
        free_ast_input(ast);
        ast = parse_input(lexer);
    }

    res = eval_input(ast);

    free_ast_input(ast);
    free_lexer(lexer);

    return res;
}
