/*
#include <stdio.h>
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

    // Print the next statement in the list (if any)
    if (node->next) {
        print_ast(node->next, level);
    }
}

int main() {
    // 1. The Code to Compile
    const char *code = "int x = 10; while (x > 0) { print x; x = x - 1; }";
    printf("----------------------------------------\n");
    printf("INPUT CODE:\n%s\n", code);
    printf("----------------------------------------\n\n");

    // 2. Initialize Lexer
    Lexer lexer;
    lexer_init(&lexer, code);
    
    // 3. Run Lexer
    TokenNode *tokens = lexer_tokenize_file(&lexer);

    // 4. PRINT LEXER OUTPUT (Added this back for you)
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

    // 5. Run Parser
    printf("=== PARSER OUTPUT (Abstract Syntax Tree) ===\n");
    Parser parser;
    parser_init(&parser, tokens);
    ASTNode *root = parser_parse(&parser);

    // 6. Print the Tree
    print_ast(root, 0);

    // Cleanup
    free_token_list(tokens);
    return 0;
}
