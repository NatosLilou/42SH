#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdio.h>

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
    AST_ELSE_CLAUSE,
    AST_COMPOUND_LIST
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
    struct ast_pipeline *pipeline;
};

struct ast_and_or *new_ast_and_or(void);
void print_ast_and_or(struct ast_and_or *ast);
void free_ast_and_or(struct ast_and_or *ast);

/*==============================   AST_PIPELINE   ===========================*/

struct ast_pipeline
{
    enum ast_type type;
    struct ast_command *command;
};

struct ast_pipeline *new_ast_pipeline(void);
void print_ast_pipeline(struct ast_pipeline *ast);
void free_ast_pipeline(struct ast_pipeline *ast);

/*==============================   AST_COMMAND   ============================*/

struct ast_command
{
    enum ast_type type;
    struct ast_simple_command *simple_command;
    struct ast_shell_command *shell_command;
};

struct ast_command *new_ast_command(void);
void print_ast_command(struct ast_command *ast);
void free_ast_command(struct ast_command *ast);

/*==============================   AST_SIMPLE_COMMAND   =====================*/

struct ast_simple_command
{
    enum ast_type type;
    char **commands; // /!\ MUST BE NULL TERMINATED
    size_t size;
    size_t pos;
};

struct ast_simple_command *new_ast_simple_command(char *command);
void add_ast_simple_command(struct ast_simple_command *ast, char *element);
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
    struct ast_and_or *and_or;
};

struct ast_compound_list *new_ast_compound_list(void);
void print_ast_compound_list(struct ast_compound_list *ast);
void free_ast_compound_list(struct ast_compound_list *ast);

#endif /* !AST_H */
