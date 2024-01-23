#ifndef EVAL_H
#define EVAL_H

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast/ast.h"
#include "execution.h"

// -------------------------- EVAL -----------------------------
#include "expansion/expansion.h"

int eval_input(struct ast_input *ast);
int eval_list(struct ast_list *ast);
int eval_and_or(struct ast_and_or *ast);
int eval_pipeline(struct ast_pipeline *ast);
int eval_command(struct ast_command *ast);
int eval_simple_command(struct ast_simple_command *ast);
int eval_shell_command(struct ast_shell_command *ast);
int eval_rule_if(struct ast_rule_if *ast);
int eval_rule_until(struct ast_rule_until *ast);
int eval_rule_while(struct ast_rule_while *ast);
int eval_else_clause(struct ast_else_clause *ast);
int eval_compound_list(struct ast_compound_list *ast);
int eval_pipeline(struct ast_pipeline *ast);
void eval_prefix(struct ast_prefix *ast);
int eval_redir(struct ast_redir *ast);
// int eval_builtin(); //TODO

#endif /* !EVAL_H */
