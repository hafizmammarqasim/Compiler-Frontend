#ifndef PARSER_H
#define PARSER_H

#include "../lexer/token.h"
#include "../lexer/lexer.h"

typedef struct {
    TokenNode *current; // pointer to current token in linked list
} Parser;

void parser_init(Parser *parser, TokenNode *head);
Token* parser_next_token(Parser *parser);
void parse_program(Parser *parser);

#endif
