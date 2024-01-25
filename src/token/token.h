#ifndef TOKEN_H
#define TOKEN_H

// Delimiters :
// & | < > ; <space> ( ) \n

enum token_type
{
    TOKEN_WORD,
    TOKEN_ASSIGNMENT_WORD,
    TOKEN_NEWLINE, // \n
    TOKEN_IO_NUMBER,
    /**** Operators ****/
    TOKEN_AND_IF, // &&
    TOKEN_OR_IF, // ||
    TOKEN_DSEMI, // ;;
    TOKEN_DLESS, // <<  default 0
    TOKEN_DGREAT, // >>  default 1
    TOKEN_LESSAND, // <&  default 0
    TOKEN_GREATAND, // >&  default 1
    TOKEN_LESSGREAT, // <>  default 0
    TOKEN_DLESSDASH, // <<-  default 0
    TOKEN_CLOBBER, // >|  default 1
    /**** Reserved words ****/
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_FI,
    TOKEN_DO,
    TOKEN_DONE,
    // TOKEN_CASE,
    // TOKEN_ESAC,
    TOKEN_WHILE,
    TOKEN_UNTIL,
    TOKEN_FOR,
    /**** Reserved words recognized when reserved word is recognized ****/
    // TOKEN_LBRACE, // {
    // TOKEN_RBRACE, // }
    TOKEN_BANG, // !
    TOKEN_IN,
    /**** Others ****/
    TOKEN_AND, // &
    TOKEN_LESS, // <  default 0
    TOKEN_GREAT, // >  default 1
    TOKEN_SEMI, // ;
    TOKEN_PIPE, // |
    TOKEN_LPAR, // (
    TOKEN_RPAR, // )
    TOKEN_EOF
};

struct token
{
    enum token_type type; // The kind of token
    char *value;
};

void free_token(struct token *tok);

#endif /* !TOKEN_H */
