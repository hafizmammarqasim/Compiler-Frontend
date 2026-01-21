#ifndef PARSER_H
#define PARSER_H

#include "../lexer/token.h"

typedef struct {
    Token *tokens;
    int pos;
} Parser;

void parser_init(Parser *parser, Token *tokens);
void parse_program(Parser *parser);

#endif
