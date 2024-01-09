#ifndef TOKEN_H
#define TOKEN_H

enum token_type
{
    TOKEN_SHELL_CMD
};

struct token
{
    enum token_type type; // The kind of token
    char *option[];       // The eventual options
};
#endif /* !TOKEN_H */
