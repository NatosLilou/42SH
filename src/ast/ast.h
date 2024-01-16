#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdio.h>

#include "../token/token.h"

enum ast_type
{
    AST_INPUT,
    AST_LIST,
    AST_AND_OR,
    AST_PIPELINE,
    AST_COMMAND,
    AST_SIMPLE_COMMAND,
    AST_SHELL_COMMAND,
    AST_RULE_IF,
    AST_RULE_WHILE,
    AST_RULE_UNTIL,
    AST_ELSE_CLAUSE,
    AST_COMPOUND_LIST,
    AST_PREFIX,
    AST_REDIR,
};

/*==============================   AST_INPUT   ==============================*/

struct ast_input
{
    enum ast_type type;
    struct ast_list *list;
    bool eof;
};

struct ast_input *new_ast_input(void);
void print_ast_input(struct ast_input *ast);
void free_ast_input(struct ast_input *ast);

/*==============================   AST_LIST   ===============================*/

struct ast_list
{
    enum ast_type type;
    struct ast_and_or **and_or; // List of ast_and_or
    size_t size;
    size_t pos;
};

struct ast_list *new_ast_list(void);
void add_ast_list(struct ast_list *ast, struct ast_and_or *baby);
void print_ast_list(struct ast_list *ast);
void free_ast_list(struct ast_list *ast);

/*==============================   AST_AND_OR   =============================*/

struct ast_and_or
{
    enum ast_type type;
    struct ast_pipeline **pipeline;
    int op; // 0: default, 1: and, 2: or
    size_t size;
    size_t pos;
};

struct ast_and_or *new_ast_and_or(void);
void add_ast_and_or(struct ast_and_or *ast, struct ast_pipeline *baby);
void print_ast_and_or(struct ast_and_or *ast);
void free_ast_and_or(struct ast_and_or *ast);

/*==============================   AST_PIPELINE   ===========================*/

struct ast_pipeline
{
    enum ast_type type;
    bool negation;
    struct ast_command **commands;
    size_t size;
    size_t pos;
};

struct ast_pipeline *new_ast_pipeline(void);
void add_ast_pipeline(struct ast_pipeline *ast, struct ast_command *command);
void print_ast_pipeline(struct ast_pipeline *ast);
void free_ast_pipeline(struct ast_pipeline *ast);

/*==============================   AST_COMMAND   ============================*/

struct ast_command
{
    enum ast_type type;
    struct ast_simple_command *simple_command;
    struct ast_shell_command *shell_command;
    struct ast_redir **redir;
    size_t size;
    size_t pos;
};

struct ast_command *new_ast_command(void);
void add_ast_command(struct ast_command *ast, struct ast_redir *redir);
void print_ast_command(struct ast_command *ast);
void free_ast_command(struct ast_command *ast);

/*========================= AST_SIMPLE_COMMAND ==============================*/

struct ast_simple_command
{
    enum ast_type type;
    struct ast_prefix **prefix;
    size_t size_pref;
    size_t pos_pref;
    char **commands; // /!\ MUST BE NULL TERMINATED
    size_t size_cmd;
    size_t pos_cmd;
    struct ast_redir **redir;
    size_t size_redir;
    size_t pos_redir;
};

struct ast_simple_command *new_ast_simple_command(void);
void add_ast_simple_command_pref(struct ast_simple_command *ast, struct ast_prefix *prefix);
void add_ast_simple_command_cmd(struct ast_simple_command *ast, char *command);
void add_ast_simple_command_redir(struct ast_simple_command *ast, struct ast_redir *redir);
void print_ast_simple_command(struct ast_simple_command *ast);
void free_ast_simple_command(struct ast_simple_command *ast);

/*=========================   AST_SHELL_COMMAND   ===========================*/

struct ast_shell_command
{
    enum ast_type type;
    struct ast_rule_if *rule_if;
};

struct ast_shell_command *new_ast_shell_command(void);
void print_ast_shell_command(struct ast_shell_command *ast);
void free_ast_shell_command(struct ast_shell_command *ast);

/*==============================   AST_RULE_IF   ============================*/

struct ast_rule_if
{
    enum ast_type type;
    struct ast_compound_list *compound_list_if;
    struct ast_compound_list *compound_list_then;
    struct ast_else_clause *else_clause;
};

struct ast_rule_if *new_ast_rule_if(void);
void print_ast_rule_if(struct ast_rule_if *ast);
void free_ast_rule_if(struct ast_rule_if *ast);

/*============================  AST_RULE_WHILE   ============================*/

struct ast_rule_while
{
    enum ast_type type;
    struct ast_compound_list *compound_list_while;
    struct ast_compound_list *compound_list_do;
};

struct ast_rule_while *new_ast_rule_while(void);
void print_ast_rule_while(struct ast_rule_while *ast);
void free_ast_rule_while(struct ast_rule_while *ast);

/*============================  AST_RULE_UNTIL   ============================*/

struct ast_rule_until
{
    enum ast_type type;
    struct ast_compound_list *compound_list_until;
    struct ast_compound_list *compound_list_do;
};

struct ast_rule_until *new_ast_rule_until(void);
void print_ast_rule_until(struct ast_rule_until *ast);
void free_ast_rule_until(struct ast_rule_until *ast);

/*==========================   AST_ELSE_CLAUSE   ============================*/

struct ast_else_clause
{
    enum ast_type type;
    struct ast_compound_list *compound_list_elif;
    struct ast_compound_list *compound_list_then;
    struct ast_else_clause *else_clause;
};

struct ast_else_clause *new_ast_else_clause(void);
void print_ast_else_clause(struct ast_else_clause *ast);
void free_ast_else_clause(struct ast_else_clause *ast);

/*========================   AST_COMPOUND_LIST   ============================*/

struct ast_compound_list
{
    enum ast_type type;
    struct ast_and_or **and_or; // List of ast_and_or
    size_t size;
    size_t pos;
};

struct ast_compound_list *new_ast_compound_list(void);
void add_ast_compound_list(struct ast_compound_list *ast,
                           struct ast_and_or *baby);
void print_ast_compound_list(struct ast_compound_list *ast);
void free_ast_compound_list(struct ast_compound_list *ast);

/*========================   AST_PREFIX =====================================*/

struct ast_prefix
{
    enum ast_type type;
    struct ast_redir *redir;
};

struct ast_prefix *new_ast_prefix(void);
void print_ast_prefix(struct ast_prefix *ast);
void free_ast_prefix(struct ast_prefix *ast);

/*========================   AST_REDIRECTION ================================*/

struct ast_redir
{
    enum ast_type type;
    int ionumber; // 0 by default (stdin) // Colle a la redirection !!!!!!
    enum token_type redir;
    char *dest;
};

struct ast_redir *new_ast_redir(void);
void print_ast_redir(struct ast_redir *ast);
void free_ast_redir(struct ast_redir *ast);

#endif /* !AST_H */
