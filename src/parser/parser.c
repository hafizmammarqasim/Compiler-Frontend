#include "parser.h"
#include <stdio.h>

void parser_init(Parser *parser, Token *tokens) {
    parser->tokens = tokens;
    parser->pos = 0;
}

void parse_program(Parser *parser) {
    (void)parser;
    printf("Parser: Program parsing placeholder\n");
}
