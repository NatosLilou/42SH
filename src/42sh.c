#include <eval.h>
#include <io_back_end.h>
#include <parser.h>

int main(int argc, char *argv[])
{
    FILE *stream = io_back_end_init(argc, argv);
    if (!stream)
    {
        return 2;
    }

    struct lexer *lexer = new_lexer(stream);

    struct ast_input *ast = parse_input(lexer);

    int res = 1;
    while (ast && !ast->eof)
    {
        res = eval_input(ast);
        free_ast_input(ast);
        ast = parse_input(lexer);
    }

    if (ast)
    {
        res = eval_input(ast);
    }
    else
    {
        free_ast_input(ast);
        io_back_end_close(lexer->stream);
        free_lexer(lexer);
        fprintf(stderr, "42sh: syntax error\n");
        return 2;
    }

    free_ast_input(ast);
    io_back_end_close(lexer->stream);
    free_lexer(lexer);

    return res;
}
