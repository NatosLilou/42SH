#define _POSIX_C_SOURCE 200809L

#include <eval.h>
#include <io_back_end.h>
#include <parser.h>

struct assigned_var *assigned = NULL;

static struct assigned_var *init_assigned(int argc)
{
    struct assigned_var *temp = malloc(sizeof(struct assigned_var));
    temp->name = calloc(1, sizeof(char *));
    temp->value = calloc(1, sizeof(char *));
    temp->pos = 0;
    temp->args = calloc(argc, sizeof(char *));
    temp->pos_args = 0;
    temp->pos_fun = 0;
    temp->fun_name = calloc(1, sizeof(char *));
    temp->fun_value = calloc(1, sizeof(struct ast_shell_command *));
    temp->pos_fun_args = 0;
    temp->fun_args = calloc(1, sizeof(char *));
    temp->in_func = false;
    temp->seed = getpid();
    temp->exit_code = 0;
    temp->exiting = 0;
    temp->shell_commands = calloc(1000, sizeof(struct ast_shell_command *));
    temp->pos_shell = 0;
    return temp;
}

static void free_all(struct ast_input *ast, struct lexer *lexer, struct io *io)
{
    free_ast_input(ast);
    io_back_end_close(io);
    free_lexer(lexer);
    if (assigned)
    {
        if (assigned->name)
        {
            for (size_t i = 0; i < assigned->pos; i++)
            {
                free(assigned->name[i]);
            }
            free(assigned->name);
        }
        if (assigned->value)
        {
            for (size_t i = 0; i < assigned->pos; i++)
            {
                if (assigned->value[i])
                    free(assigned->value[i]);
            }
            free(assigned->value);
        }
        if (assigned->args)
        {
            /*
            for (size_t i = 0; i < assigned->pos_args; i++)
            {
                free(assigned->args[i]);
            }*/
            free(assigned->args);
        }
        if (assigned->fun_name)
        {
            for (size_t i = 0; i < assigned->pos_fun; i++)
            {
                free(assigned->fun_name[i]);
            }
            free(assigned->fun_name);
        }
        if (assigned->fun_value)
        {
            /*for (size_t i = 0; i < assigned->pos_fun; i++)
            {
                free_ast_shell_command(
                    (struct ast_shell_command *)assigned->fun_value[i]);
            }*/
            free(assigned->fun_value);
        }
        if (assigned->shell_commands)
        {
            for (size_t i = 0; i < assigned->pos_shell; i++)
            {
                free_ast_shell_command(
                    (struct ast_shell_command *)assigned->shell_commands[i]);
            }
            free(assigned->shell_commands);
        }

        free(assigned->fun_args);
        free(assigned);
    }
}

static void init_env(void)
{
    char *cwd = malloc(1000 * sizeof(char));
    cwd = getcwd(cwd, 1000);
    setenv("PWD", cwd, 1);
    free(cwd);
    setenv("IFS", " \t\n", 1);
}

int main(int argc, char *argv[])
{
    init_env();
    assigned = init_assigned(argc);
    struct io *io = io_back_end_init(argc, argv);
    if (!io)
    {
        return 2;
    }
    if (io->isatty)
    {
        printf("\033[0;34m%s \033[1;32m42sh$ \033[0m", getenv("PWD")); // PS1
    }

    struct lexer *lexer = new_lexer(io);

    struct ast_input *ast = parse_input(lexer);

    int res = 0;
    while (ast && !ast->eof)
    {
        if ((res = eval_input(ast)) == -42)
        {
            res = assigned->exiting;
            goto exit;
        }
        else if (res <= -50)
        {
            res = assigned->exiting;
        }

        assigned->exit_code = res;

        free_ast_input(ast);
        if (io->isatty)
        {
            printf("\033[0;34m%s \033[1;32m42sh$ \033[0m", getenv("PWD"));
        }
        ast = parse_input(lexer);
    }

    if (ast)
    {
        if (ast->list)
        {
            if ((res = eval_input(ast)) == -42)
                res = assigned->exiting;
            else if (res <= -50)
            {
                res = assigned->exiting;
            }
        }
    }
    else
    {
        free_all(ast, lexer, io);
        fprintf(stderr, "42sh: syntax error\n");
        return 2;
    }

exit:
    free_all(ast, lexer, io);

    return res;
}
