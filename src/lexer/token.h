#ifndef TOKEN_H
#define TOKEN_H

// Delimiters :
// & | < > ; <space> ( ) \n

enum token_type
{
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_OPERATOR,
    TOKEN_WORD
};

struct token
{
    enum token_type type; // The kind of token
    char *value;
};

#endif /* !TOKEN_H */
