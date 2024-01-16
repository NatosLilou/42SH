#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

#include <stdbool.h>
   
struct ast_input *parse_input(struct lexer *lexer);

struct ast_list *parse_list(struct lexer *lexer);

struct ast_and_or *parse_and_or(struct lexer *lexer);

struct ast_pipeline *parse_pipeline(struct lexer *lexer);

struct ast_command *parse_command(struct lexer *lexer);

struct ast_simple_command *parse_simple_command(struct lexer *lexer);

struct ast_shell_command *parse_shell_command(struct lexer *lexer);

struct ast_rule_if *parse_rule_if(struct lexer *lexer);

struct ast_else_clause *parse_else_clause(struct lexer *lexer);

struct ast_compound_list *parse_compound_list(struct lexer *lexer);

struct ast_redir *parse_redir(struct lexer *lexer);

struct ast_prefix *parse_prefix(struct lexer *lexer);

struct ast_rule_while *parse_rule_while(struct lexer *lexer);

struct ast_rule_until *parse_rule_until(struct lexer *lexer);

bool parse_element(struct ast_simple_command *ast, struct lexer *lexer);

#endif /* !PARSER_H */
