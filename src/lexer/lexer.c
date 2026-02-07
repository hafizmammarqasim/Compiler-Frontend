#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// initialize lexer
void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
}

// helper: create a token
Token* create_token(TokenType type, const char *lexeme, int line) {
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->lexeme = malloc(strlen(lexeme)+1);
    strcpy(token->lexeme, lexeme);
    token->line = line;
    return token;
}

// free token linked list
void free_token_list(TokenNode *head) {
    TokenNode *current = head;
    while(current) {
        TokenNode *next = current->next;
        free(current->token->lexeme);
        free(current->token);
        free(current);
        current = next;
    }
}

// tokenize: **simplified skeleton**
TokenNode* lexer_tokenize_file(Lexer *lexer) {
    TokenNode *head = NULL;
    

    // For now: just create EOF token to test linked list
   Token *eof_token = create_token(TOK_EOF, "EOF", lexer->line);
TokenNode *node = malloc(sizeof(TokenNode));
node->token = eof_token;
node->next = NULL;
head = node;

    return head;
}

