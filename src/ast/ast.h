#ifndef AST_H
#define AST_H

enum ast_type
{
    AST_INPUT,
    AST_LIST,
    AST_AND_OR,
    AST_PIPELINE,
    AST_COMMAND,
    AST_SIMPLE_COMMAND
};

struct ast_input
{
    enum ast_type type;
    struct ast_list *list;
}

struct ast_list
{
    enum ast_type type;
    struct ast_and_or *and_or;
}

struct ast_and_or
{
    enum ast_type type;
    struct ast_pipeline *pipeline;
}

struct ast_pipeline
{
    enum ast_type type;
    struct ast_command *command;
}

struct ast_command
{
    enum ast_type type;
    struct ast_simple_command *simple_command;
}

struct ast_simple_command
{
    enum ast_type type;
    char *command[];
};

#endif /* !AST_H */
