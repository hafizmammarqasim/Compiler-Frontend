#ifndef TOKEN_H
#define TOKEN_H

typedef enum
{
    TOK_EOF,
    TOK_INVALID,
    TOK_UNKNOWN,
    TOK_IDENTIFIER,
    TOK_NUMBER, // a digit (i.e 5)
    TOK_INT,         // keyword int
    TOK_IF,
    TOK_BREAK,
    TOK_CONTINUE,
    TOK_PRINT,
    TOK_WHILE,
    TOK_ASSIGN,         // =
    TOK_PLUS,           // +
    TOK_MINUS,          // -
    TOK_STAR,           // *
    TOK_SLASH,          // /
    TOK_LPAREN,         // (
    TOK_RPAREN,         // )
    TOK_SEMICOLON,      // ;
    TOK_GREATER,        //>
    TOK_LESS,           //<
    TOK_LEQUAL,         //<=
    TOK_GEQUAL,         //>=
    TOK_EQUALITY_CHECK, //==
    TOK_LBRACE,         //{
    TOK_RBRACE,         //}
    TOK_MODULUS,        // %
    TOK_COMMA,          // ,
    TOK_NOT,            //!
    TOK_DOUBLE_QUOTE    //"
} TokenType;

typedef struct
{
    TokenType type;
    char *lexeme;
    int line;
    int column;

} Token;

typedef struct
{
    const char *keyword;
    TokenType type;
} Keyword;
extern Keyword keywords[];

typedef struct
{
    const char *operator;
    TokenType type;
} Operator;
extern Operator operators[];
#endif
