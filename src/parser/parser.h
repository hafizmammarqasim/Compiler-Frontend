#ifndef PARSER_H
#define PARSER_H

#include "../lexer/token.h"
#include "../lexer/lexer.h"

// Types of nodes in the AST
typedef enum {
    NODE_PROGRAM,
    NODE_VAR_DECLARATION, 
    NODE_ASSIGNMENT,      
    NODE_PRINT,           
    NODE_IF,              
    NODE_WHILE,           
    NODE_BLOCK,           // { ... }
    NODE_BINARY_OP,       // +, -, *, /, <, >
    NODE_NUMBER,         
    NODE_VARIABLE       
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    
    // For Variables / Numbers
    char *text; 
    
    TokenType op; 
    struct ASTNode *left;
    struct ASTNode *right;

    // For Control Flow (If / While)
    struct ASTNode *condition;
    struct ASTNode *body;
    
    struct ASTNode *next; 

} ASTNode;

typedef struct {
    TokenNode *current;
} Parser;

void parser_init(Parser *parser, TokenNode *head);
ASTNode* parser_parse(Parser *parser);

#endif