#ifndef EVAL_H
#define EVAL_H

#include "../ast/ast.h"
#include "../execution/execution.h"

int eval_input(struct ast_input *ast);
int eval_list(struct ast_list *ast);
int eval_and_or(struct ast_and_or *ast);
int eval_pipeline(struct ast_pipeline *ast);
int eval_command(struct ast_command *ast);
int eval_simple_command(struct ast_simple_command *ast);
//int eval_shell_command(); //TODO
//int eval_builtin(); //TODO
//int eval_conpoumd_list(); //TODO

#endif /* !EVAL_H */
