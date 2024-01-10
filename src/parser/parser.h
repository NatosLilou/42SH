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
enum parser_status parse_input(struct ast **res, struct lexer *lexer);

/**
 * list =       and_or
 *          ;
 */
enum parser_status parse_list(struct ast **res, struct lexer *lexer);

/**
 * and_or =    pipeline
 *          ;
 */
enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);

/**
 * pipeline =   command
 *          ;
 */
enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);

/**
 * command =    simple_command
 *          ;
 */
enum parser_status parse_command(struct ast **res, struct lexer *lexer);

/**
 * simple_command =     WORD { element }
 *                  ;
 */
enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer);

/**
 * element =    WORD
 *          ;
 */
enum parser_status parse_element(struct ast **res, struct lexer *lexer);

#endif /* !PARSER_H */
