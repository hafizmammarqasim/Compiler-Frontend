#include <stdio.h>
#include <stdlib.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"

int main() {
    const char *code = "int x = 10; if (x > 0) { x = x + 1; }";

    Lexer lexer;
    lexer_init(&lexer, code);

    TokenNode *head = lexer_tokenize_file(&lexer);

    printf("=== LEXER ===\n");
    TokenNode *current = head;
    while (current) {
        printf("Token type: %d, lexeme: %s, line: %d\n",
            current->token->type,
            current->token->lexeme,
            current->token->line);
        current = current->next;
    }

    printf("\n=== PARSER ===\n");
    Parser parser;
    parser_init(&parser, head);
    parse_program(&parser);

    printf("\n=== SEMANTIC ===\n");
    semantic_analyze();

    free_token_list(head);
    return 0;
}
