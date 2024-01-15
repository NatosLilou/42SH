#include <eval.h>
#include <io_back_end.h>
#include <parser.h>

int main(int argc, char *argv[])
{
    struct io *io = io_back_end_init(argc, argv);
    // printf("IO OK\n");
    if (!io)
    {
        return 2;
    }
    if (io->isatty)
    {
        printf("\033[0;31m"); // Set the text to the color red
        printf("42sh$ "); // PS1
        printf("\033[0m");
    }

    struct lexer *lexer = new_lexer(io);
    // printf("LEXER OK\n");

    struct ast_input *ast = parse_input(lexer);
    // printf("PARSER OK\n");

    int res = 1;
    while (ast && !ast->eof)
    {
        // printf("LOOP\n");
        res = eval_input(ast);
        // printf("EVAL OK\n");
        free_ast_input(ast);
        if (io->isatty)
        {
            printf("\033[0;31m"); // Set the text to the color red
            printf("42sh$ "); // PS1
            printf("\033[0m");
        }
        // printf("FREE AST\n");
        ast = parse_input(lexer);
        // printf("PARSE\n");
    }

    // printf("EXIT LOOP\n");
    if (ast)
    {
        // printf("LAST EVAL OK\n");
        res = eval_input(ast);
    }
    else
    {
        // printf("FREE\n");
        free_ast_input(ast);
        // printf("FREE_AST\n");
        io_back_end_close(io);
        // printf("FREE_IO\n");
        free_lexer(lexer);
        // printf("FREE_LEX\n");
        fprintf(stderr, "42sh: syntax error\n");
        return 2;
    }

    free_ast_input(ast);
    io_back_end_close(io);
    free_lexer(lexer);

    return res;
}
