#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"
/**
 * [] = optional
 * {} = 0 to infinity
 * () = group
 */

/**
 * input =      list '\n'
 *          |   list EOF
 *          |   '\n'
 *          |   EOF
 *          ;
 */
struct ast_input *parse_input(struct lexer *lexer);

/**
 * list =       and_or
 *          ;
 */
struct ast_list *parse_list(struct lexer *lexer);

/**
 * and_or =    pipeline
 *          ;
 */
struct ast_and_or *parse_and_or(struct lexer *lexer);

/**
 * pipeline =   command
 *          ;
 */
struct ast_pipeline *parse_pipeline(struct lexer *lexer);

/**
 * command =    simple_command
 *          ;
 */
struct ast_command *parse_command(struct lexer *lexer);

/**
 * simple_command =     WORD { element }
 *                  ;
 */
struct ast_simple_command *parse_simple_command(struct lexer *lexer);

struct ast_shell_command *parse_shell_command(struct lexer *lexer);

struct ast_rule_if *parse_rule_if(struct lexer *lexer);

struct ast_else_clause *parse_else_clause(struct lexer *lexer);

struct ast_compound_list *parse_compound_list(struct lexer *lexer);

/**
 * element =    WORD
 *          ;
 */
char *parse_element(struct lexer *lexer);

#endif /* !PARSER_H */
