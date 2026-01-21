#include <stdio.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"

int main() {
    const char *code = "int x = 10;";

    printf("=== LEXER ===\n");

    Lexer lexer;
    lexer_init(&lexer, code);

    Token token = lexer_next_token(&lexer);
    printf("Token type: %d (line %d)\n", token.type, token.line);

    printf("\n=== PARSER ===\n");

    Parser parser;
    parser_init(&parser, &token);
    parse_program(&parser);

    printf("\n=== SEMANTIC ===\n");
    semantic_analyze();

    return 0;
}
