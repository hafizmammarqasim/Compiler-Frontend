#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include "../symbol_table/symbol_table.h"


void analyze_node(ASTNode *node);

// Helper function
void analyze_list(ASTNode *head) {
    ASTNode *current = head;
    while (current != NULL) {
        analyze_node(current);
        current = current->next;
    }
}

void analyze_node(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        // Containers 
        case NODE_PROGRAM:

            analyze_list(node->body);
            break;

        case NODE_BLOCK:
            enter_scope();
            analyze_list(node->body);
            exit_scope();
            break;

        // Declarations & Assignments
        case NODE_VAR_DECLARATION:
            // Analyze the RHS expression first
            analyze_node(node->right);

            // Check for redeclaration
            if (lookup_current_scope(node->text)) {
                fprintf(stderr, "Semantic Error: Variable '%s' already declared in this scope.\n", node->text);
            } else {
                insert_symbol(node->text, TYPE_INT);
            }
            break;

        case NODE_ASSIGNMENT:
            // Check if variable exists 
            if (!lookup_symbol(node->text)) {
                fprintf(stderr, "Semantic Error: Assignment to undeclared variable '%s'.\n", node->text);
            }

            analyze_node(node->right);
            break;

        case NODE_IF:
            analyze_node(node->condition);
            analyze_node(node->body); 
            break;

        case NODE_WHILE:
            analyze_node(node->condition);
            analyze_node(node->body);
            break;

        case NODE_PRINT:
            analyze_node(node->right);
            break;

        //  Expressions 
        case NODE_BINARY_OP:
            analyze_node(node->left);
            analyze_node(node->right);
            break;

        case NODE_VARIABLE:

            if (!lookup_symbol(node->text)) {
                fprintf(stderr, "Semantic Error: Use of undeclared variable '%s'.\n", node->text);
            }
            break;

        case NODE_NUMBER:
            // Always valid
            break;

        default:
            break;
    }
}

void semantic_analyze(ASTNode *root) {
    if (!root) return;

    analyze_node(root);

    print_symbol_table();
    
    free_symbol_table();
}