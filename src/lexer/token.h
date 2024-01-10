#ifndef TOKEN_H
#define TOKEN_H

// Delimiters :
// & | < > ; <space> ( ) \n

enum token_type
{
    TOKEN_WORD,
    // TOKEN_ASSIGNEMENT_WORD,
    // TOKEN_NAME,
    TOKEN_NEWLINE,
    // TOKEN_IO_NUMBER,
    /**** Operators ****/
    // TOKEN_AND_IF,
    // TOKEN_OR_IF,
    // TOKEN_DSEMI,
    // TOKEN_DLESS,
    // TOKEN_DGREAT,
    // TOKEN_LESSAND,
    // TOKEN_GREATAND,
    // TOKEN_LESSGREAT,
    // TOKEN_DLESSDASH,
    // TOKEN_CLOBBER,
    /**** Reserved words ****/
    // TOKEN_IF,
    // TOKEN_THEN,
    // TOKEN_ELSE,
    // TOKEN_ELIF,
    // TOKEN_FI,
    // TOKEN_DO,
    // TOKEN_DONE,
    // TOKEN_CASE,
    // TOKEN_ESAC,
    // TOKEN_WHILE,
    // TOKEN_UNTIL,
    // TOKEN_FOR,
    /**** Reserved words recognized when reserved word is recognized ****/
    // TOKEN_LBRACE,
    // TOKEN_RBRACE,
    // TOKEN_BANG,
    // TOKEN_IN,
    TOKEN_EOF,
    TOKEN_ERROR
};

struct token
{
    enum token_type type; // The kind of token
    char *value;
};

#endif /* !TOKEN_H */
