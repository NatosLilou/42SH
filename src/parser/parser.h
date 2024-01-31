#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct ast_input *parse_input(struct lexer *lexer);

struct ast_list *parse_list(struct lexer *lexer, bool *syntax_error,
                            int loop_stage);

struct ast_and_or *parse_and_or(struct lexer *lexer, bool *syntax_error,
                                int loop_stage);

struct ast_pipeline *parse_pipeline(struct lexer *lexer, bool *syntax_error,
                                    int loop_stage);

struct ast_command *parse_command(struct lexer *lexer, bool *syntax_error,
                                  int loop_stage);

struct ast_simple_command *
parse_simple_command(struct lexer *lexer, bool *syntax_error, int loop_stage);

struct ast_shell_command *
parse_shell_command(struct lexer *lexer, bool *syntax_error, int loop_stage);

struct ast_rule_if *parse_rule_if(struct lexer *lexer, bool *syntax_error,
                                  int loop_stage);

struct ast_else_clause *parse_else_clause(struct lexer *lexer,
                                          bool *syntax_error, int loop_stage);

struct ast_compound_list *
parse_compound_list(struct lexer *lexer, bool *syntax_error, int loop_stage);

struct ast_redir *parse_redir(struct lexer *lexer, bool *syntax_error,
                              int loop_stage);

struct ast_prefix *parse_prefix(struct lexer *lexer, bool *syntax_error,
                                int loop_stage);

struct ast_rule_while *parse_rule_while(struct lexer *lexer, bool *syntax_error,
                                        int loop_stage);

struct ast_rule_until *parse_rule_until(struct lexer *lexer, bool *syntax_error,
                                        int loop_stage);

struct ast_rule_for *parse_rule_for(struct lexer *lexer, bool *syntax_error,
                                    int loop_stage);

int parse_element(struct ast_simple_command *ast, struct lexer *lexer,
                  bool *syntax_error, int loop_stage);

struct ast_funcdec *parse_ast_funcdec(struct lexer *lexer, bool *syntax_error,
                                      int loop_stage);

struct ast_rule_case *parse_rule_case(struct lexer *lexer, bool *syntax_error, int loop_stage);

struct ast_case_clause *parse_case_clause(struct lexer *lexer, bool *syntax_error, int loop_stage);

struct ast_case_item *parse_case_item(struct lexer *lexer, bool *syntax_error, int loop_stage);

#endif /* !PARSER_H */
