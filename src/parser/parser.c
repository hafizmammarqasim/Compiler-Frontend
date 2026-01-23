#include "parser.h"
#include <stdio.h>

void parser_init(Parser *parser, TokenNode *head) {
    parser->current = head;
}

Token* parser_next_token(Parser *parser) {
    if (!parser->current) return NULL;
    Token *tok = parser->current->token;
    parser->current = parser->current->next;
    return tok;
}

// simple placeholder to print tokens
void parse_program(Parser *parser) {
    Token *tok;
    printf("Parser: Tokens in program:\n");
    while ((tok = parser_next_token(parser)) != NULL) {
        printf("Token type: %d, lexeme: %s, line: %d\n",
                tok->type, tok->lexeme, tok->line);
    }
}
