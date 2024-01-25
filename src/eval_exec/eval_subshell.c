#include "eval.h"

int eval_subshell(struct ast_compound_list *ast)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        if (eval_compound_list(ast) == 0)
        {
            exit(EXIT_SUCCESS);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        int wstatus;
        waitpid(pid,&wstatus,0);
        return WEXITSTATUS(wstatus);
    }
}
