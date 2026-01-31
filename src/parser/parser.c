#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

Token* current_token(Parser *p) {
    if (p->current == NULL) return NULL;
    return p->current->token;
}

void eat(Parser *p, TokenType type) {
    Token *t = current_token(p);
    if (t != NULL && t->type == type) {
        p->current = p->current->next;
    } else {
        printf("Syntax Error: Expected token type %d but got %s\n", 
               type, t ? TokenType_To_String(t->type) : "END");
        exit(1); 
    }
}

int check(Parser *p, TokenType type) {
    Token *t = current_token(p);
    return (t != NULL && t->type == type);
}

// new AST Node
ASTNode* create_node(ASTNodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->text = NULL;
    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    node->body = NULL;
    node->next = NULL;
    return node;
}

ASTNode* parse_expression(Parser *p);
ASTNode* parse_statement(Parser *p);
ASTNode* parse_block(Parser *p);


ASTNode* parse_factor(Parser *p) {
    Token *t = current_token(p);

    if (check(p, TOK_NUMBER)) {
        ASTNode *node = create_node(NODE_NUMBER);
        node->text = strdup(t->lexeme); // Save the number 
        eat(p, TOK_NUMBER);
        return node;
    } 
    else if (check(p, TOK_IDENTIFIER)) {
        ASTNode *node = create_node(NODE_VARIABLE);
        node->text = strdup(t->lexeme); // Save the name
        eat(p, TOK_IDENTIFIER);
        return node;
    } 
    else if (check(p, TOK_LPAREN)) {
        eat(p, TOK_LPAREN);
        ASTNode *node = parse_expression(p);
        eat(p, TOK_RPAREN);
        return node;
    }
    
    printf("Syntax Error: Unexpected token in expression: %s\n", t->lexeme);
    exit(1);
}

ASTNode* parse_term(Parser *p) {
    ASTNode *left = parse_factor(p);

    while (check(p, TOK_STAR) || check(p, TOK_SLASH) || check(p, TOK_MODULUS)) {
        TokenType op = current_token(p)->type;
        eat(p, op);
        
        ASTNode *node = create_node(NODE_BINARY_OP);
        node->op = op;
        node->left = left;
        node->right = parse_factor(p);
        left = node;
    }
    return left;
}

ASTNode* parse_additive(Parser *p) {
    ASTNode *left = parse_term(p);

    while (check(p, TOK_PLUS) || check(p, TOK_MINUS)) {
        TokenType op = current_token(p)->type;
        eat(p, op);

        ASTNode *node = create_node(NODE_BINARY_OP);
        node->op = op;
        node->left = left;
        node->right = parse_term(p);
        left = node;
    }
    return left;
}

ASTNode* parse_expression(Parser *p) {
    ASTNode *left = parse_additive(p);

    if (check(p, TOK_GREATER) || check(p, TOK_LESS) || 
        check(p, TOK_EQUALITY_CHECK) || check(p, TOK_GEQUAL) || check(p, TOK_LEQUAL)) {
        
        TokenType op = current_token(p)->type;
        eat(p, op);

        ASTNode *node = create_node(NODE_BINARY_OP);
        node->op = op;
        node->left = left;
        node->right = parse_additive(p);
        return node;
    }
    return left;
}


ASTNode* parse_statement(Parser *p) {

    if (check(p, TOK_INT)) {
        eat(p, TOK_INT);
        Token *id = current_token(p);
        eat(p, TOK_IDENTIFIER);
        eat(p, TOK_ASSIGN);
        
        ASTNode *node = create_node(NODE_VAR_DECLARATION);
        node->text = strdup(id->lexeme);
        node->right = parse_expression(p); 
        
        eat(p, TOK_SEMICOLON);
        return node;
    }
    // 2. Print Statement: print x;
    else if (check(p, TOK_PRINT)) {
        eat(p, TOK_PRINT);
        ASTNode *node = create_node(NODE_PRINT);
        node->right = parse_expression(p);
        eat(p, TOK_SEMICOLON);
        return node;
    }
    // 3. While Loop: while (x < 10) { ... }
    else if (check(p, TOK_WHILE)) {
        eat(p, TOK_WHILE);
        eat(p, TOK_LPAREN);
        ASTNode *cond = parse_expression(p);
        eat(p, TOK_RPAREN);
        
        ASTNode *node = create_node(NODE_WHILE);
        node->condition = cond;
        node->body = parse_block(p);
        return node;
    }
    // 4. If Statement: if (x > 10) { ... }
    else if (check(p, TOK_IF)) {
        eat(p, TOK_IF);
        eat(p, TOK_LPAREN);
        ASTNode *cond = parse_expression(p);
        eat(p, TOK_RPAREN);

        ASTNode *node = create_node(NODE_IF);
        node->condition = cond;
        node->body = parse_block(p);
        return node;
    }
    // 5. Assignment: x = 10;
    else if (check(p, TOK_IDENTIFIER)) {
        Token *id = current_token(p);
        eat(p, TOK_IDENTIFIER);
        eat(p, TOK_ASSIGN);
        
        ASTNode *node = create_node(NODE_ASSIGNMENT);
        node->text = strdup(id->lexeme);
        node->right = parse_expression(p);
        
        eat(p, TOK_SEMICOLON);
        return node;
    }

    printf("Syntax Error: Unexpected token %s\n", current_token(p)->lexeme);
    exit(1);
}

// Parse a block of code { ... }
ASTNode* parse_block(Parser *p) {
    eat(p, TOK_LBRACE);
    
    ASTNode *head = NULL;
    ASTNode *current = NULL;

    //parse statements until we hit '}'
    while (!check(p, TOK_RBRACE) && !check(p, TOK_EOF)) {
        ASTNode *stmt = parse_statement(p);
        
        if (head == NULL) {
            head = stmt;
            current = stmt;
        } else {
            current->next = stmt;
            current = stmt;
        }
    }

    eat(p, TOK_RBRACE);
    
    ASTNode *block = create_node(NODE_BLOCK);
    block->body = head; 
    return block;
}

// --- Entry Point ---

void parser_init(Parser *p, TokenNode *head) {
    p->current = head;
}

ASTNode* parser_parse(Parser *p) {
    ASTNode *head = NULL;
    ASTNode *current = NULL;

    while (!check(p, TOK_EOF)) {
        ASTNode *stmt = parse_statement(p);
        
        if (head == NULL) {
            head = stmt;
            current = stmt;
        } else {
            current->next = stmt;
            current = stmt;
        }
    }
    
    ASTNode *program = create_node(NODE_PROGRAM);
    program->body = head;
    return program;
}