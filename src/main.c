#include <stdio.h>
#include <stdlib.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"
#include "symbol_table/symbol_table.h"

// Helper to print indentation for the Tree
void print_indent(int level) {
    for (int i = 0; i < level; i++) printf("  ");
}

// Recursive function to print the AST
void print_ast(ASTNode *node, int level) {
    if (!node) return;

    print_indent(level);

    switch (node->type) {
        case NODE_PROGRAM:
            printf("PROGRAM\n");
            print_ast(node->body, level + 1);
            break;
        case NODE_VAR_DECLARATION:
            printf("DECLARE INT %s\n", node->text);
            print_indent(level + 1); printf("Value:\n");
            print_ast(node->right, level + 2);
            break;
        case NODE_ASSIGNMENT:
            printf("ASSIGN %s\n", node->text);
            print_indent(level + 1); printf("Value:\n");
            print_ast(node->right, level + 2);
            break;
        case NODE_PRINT:
            printf("PRINT\n");
            print_ast(node->right, level + 1);
            break;
        case NODE_IF:
            printf("IF\n");
            print_indent(level+1); printf("Condition:\n");
            print_ast(node->condition, level + 2);
            print_indent(level+1); printf("Body:\n");
            print_ast(node->body, level + 2);
            break;
        case NODE_WHILE:
            printf("WHILE\n");
            print_indent(level+1); printf("Condition:\n");
            print_ast(node->condition, level + 2);
            print_indent(level+1); printf("Body:\n");
            print_ast(node->body, level + 2);
            break;
        case NODE_BLOCK:
            printf("BLOCK {\n");
            print_ast(node->body, level + 1);
            print_indent(level); printf("}\n");
            break;
        case NODE_BINARY_OP:
            printf("OP %s\n", TokenType_To_String(node->op));
            print_ast(node->left, level + 1);
            print_ast(node->right, level + 1);
            break;
        case NODE_NUMBER:
            printf("NUMBER: %s\n", node->text);
            break;
        case NODE_VARIABLE:
            printf("VAR: %s\n", node->text);
            break;
        default:
            printf("UNKNOWN NODE\n");
    }

    if (node->next) {
        print_ast(node->next, level);
    }
}

int main() {
  
    const char *code = "int x = 10; x = x + 1; { int y = 20; print y; } print x;";
    printf("----------------------------------------\n");
    printf("INPUT CODE:\n%s\n", code);
    printf("----------------------------------------\n\n");


    Lexer lexer;
    lexer_init(&lexer, code);
    TokenNode *tokens = lexer_tokenize_file(&lexer);

    printf("=== LEXER OUTPUT ===\n");
    TokenNode *current = tokens;
    while (current) {
        if (current->token->type != TOK_EOF) {
            printf("[Line %d] %-15s : %s\n", 
                current->token->line,
                TokenType_To_String(current->token->type),
                current->token->lexeme);
        }
        current = current->next;
    }
    printf("----------------------------------------\n\n");

    // Parser
    printf("=== PARSER OUTPUT (AST) ===\n");
    Parser parser;
    parser_init(&parser, tokens);
    
    ASTNode *root = parser_parse(&parser);
    
    print_ast(root, 0);

    // Semantic Analysis
    printf("\n=== SEMANTIC ANALYSIS ===\n");
    
    // Pass the root node
    semantic_analyze(root);

    free_token_list(tokens);
    return 0;
}