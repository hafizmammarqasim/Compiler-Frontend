#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct
{
    const char *source;
    int pos;
    int line;
    int column;
} Lexer;

typedef struct TokenNode
{
    Token *token;
    struct TokenNode *next;
} TokenNode;

void lexer_init(Lexer *lexer, const char *source);
TokenNode *lexer_tokenize_file(Lexer *lexer);
void free_token_list(TokenNode *head);
const char *TokenType_To_String(TokenType type);
#endif
