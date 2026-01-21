#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOK_EOF,
    TOK_INVALID,
    TOK_IDENTIFIER,
    TOK_INT_LITERAL,
    TOK_LET,
    TOK_IF,
    TOK_THEN,
    TOK_END,
    TOK_PRINT,
    TOK_ASSIGN,   // =
    TOK_PLUS,     // +
    TOK_MINUS,    // -
    TOK_STAR,     // *
    TOK_SLASH,    // /
    TOK_LPAREN,   // (
    TOK_RPAREN,   // )
    TOK_SEMICOLON // ;
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
    int line;
} Token;

#endif
