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

/*==============================   AST_INPUT   ==============================*/

struct ast_input
{
    enum ast_type type;
    struct ast_list *list;
};

struct ast_input *new_ast_input(void);
void free_ast_input(struct ast_input *ast);

/*==============================   AST_LIST   ===============================*/

struct ast_list
{
    enum ast_type type;
    struct ast_and_or *and_or;
};

struct ast_list *new_ast_list(void);
void free_ast_list(struct ast_list *ast);

/*==============================   AST_AND_OR   =============================*/

struct ast_and_or
{
    enum ast_type type;
    struct ast_pipeline *pipeline;
};

struct ast_and_or *new_ast_and_or(void);
void free_ast_and_or(struct ast_and_or *ast);

/*==============================   AST_PIPELINE   ===========================*/

struct ast_pipeline
{
    enum ast_type type;
    struct ast_command *command;
};

struct ast_pipeline *new_ast_pipeline(void);
void free_ast_pipeline(struct ast_pipeline *ast);

/*==============================   AST_COMMAND   ============================*/

struct ast_command
{
    enum ast_type type;
    struct ast_simple_command *simple_command;
};

struct ast_command *new_ast_command(void);
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
void add_ast_simple_command(struct ast_simple_commant *ast, char *element);
void free_ast_simple_command(struct ast_simple_command *ast);

#endif /* !AST_H */
