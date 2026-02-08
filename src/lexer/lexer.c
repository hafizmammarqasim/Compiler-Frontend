#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// initialize lexer
void lexer_init(Lexer *lexer, const char *source)
{
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
}

Token *create_token(TokenType type, char *lexeme, int line, int column)
{
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = type;
    token->lexeme = lexeme;
    token->line = line;
    token->column = column;
    return token;
}

// free token linked list
void free_token_list(TokenNode *head)
{
    TokenNode *current = head;
    while (current)
    {
        TokenNode *next = current->next;
        if (current->token->lexeme)
        {
            free(current->token->lexeme);
        }
        free(current->token);
        free(current);
        current = next;
    }
}

Keyword keywords[] = {
    {"if", TOK_IF},
    {"int", TOK_INT},
    {"break", TOK_BREAK},
    {"continue", TOK_CONTINUE},
    {"print", TOK_PRINT},
    {"while", TOK_WHILE},
    {"return", TOK_RETURN},
    {NULL, TOK_INVALID} 
};

Operator operators[] = {
    {"==", TOK_EQUALITY_CHECK},
    {"!=", TOK_NOT_EQUAL},
    {">=", TOK_GEQUAL},
    {"<=", TOK_LEQUAL},
    {"=", TOK_ASSIGN},
    {"+", TOK_PLUS},
    {"-", TOK_MINUS},
    {"*", TOK_STAR},
    {"/", TOK_SLASH},
    {"<", TOK_LESS},
    {">", TOK_GREATER},
    {";", TOK_SEMICOLON},
    {"%", TOK_MODULUS},
    {"{", TOK_LBRACE},
    {"}", TOK_RBRACE},
    {",", TOK_COMMA},
    {"!", TOK_NOT},
    {"(", TOK_LPAREN},
    {")", TOK_RPAREN},
    {NULL, TOK_INVALID}

};

// Check if the Token is a Keyword
TokenType Get_Keyword_Type(const char *lexeme)
{
    for (int i = 0; keywords[i].keyword != NULL; i++)
    {
        if (strcmp(lexeme, keywords[i].keyword) == 0)
        {
            return keywords[i].type;
        }
    }
    return TOK_UNKNOWN;
};

// Checks if the Token is an Operator
TokenType Get_Operator_Type(const char *lexeme)
{
    for (int i = 0; operators[i].operator != NULL; i++)
    {
        if (strcmp(lexeme, operators[i].operator) == 0)
        {
            return operators[i].type;
        }
    }
    return TOK_INVALID;
}
TokenType Get_Number_Type(const char *lexeme)
{
    if (lexeme[0] == '\0')
    {
        return TOK_INVALID;
    }

    for (int i = 0; lexeme[i] != '\0'; i++)
    {
        if (!isdigit(lexeme[i]))
        {
            return TOK_INVALID;
        }
    }
    return TOK_NUMBER;
}
TokenType Get_Identifier_Type(const char *lexeme)
{
    if (!(isalpha(lexeme[0]) || lexeme[0] == '_'))
        return TOK_INVALID;

    for (int i = 1; lexeme[i]; i++)
    {
        if (!isalnum(lexeme[i]) && lexeme[i] != '_')
            return TOK_INVALID;
    }

    return TOK_IDENTIFIER;
}

// Check the Token Type
TokenType Determine_Tokentype(char *lexeme)
{
    TokenType type;

    type = Get_Keyword_Type(lexeme);
    if (type != TOK_UNKNOWN)
        return type;

    type = Get_Operator_Type(lexeme);
    if (type != TOK_INVALID)
        return type;

    type = Get_Number_Type(lexeme);
    if (type == TOK_NUMBER)
        return type;

    type = Get_Identifier_Type(lexeme);
    if (type == TOK_IDENTIFIER)
        return type;

    return TOK_UNKNOWN;
}

TokenNode *lexer_tokenize_file(Lexer *lexer)
{

    TokenNode *head = NULL;
    TokenNode *tail = NULL;

    while (lexer->source[lexer->pos] != '\0')
    {
        char c = lexer->source[lexer->pos];

        if (isspace(c))
        {
            if (c == '\n')
            {
                lexer->line++;
                lexer->column = 1;
            }
            else
            {
                lexer->column++;
            }

            lexer->pos++;
            continue;
        }
        int start_column = lexer->column;
        int start_pos = lexer->pos;

        if (c == '"')
        {
            lexer->pos++;
            lexer->column++;
            int start = lexer->pos;

            while (lexer->source[lexer->pos] && lexer->source[lexer->pos] != '"')
            {
                lexer->pos++;
                lexer->column++;
            }

            if (lexer->source[lexer->pos] != '"')
            {
                fprintf(stderr, "Unterminated string at line %d\n", lexer->line);
                break;
            }

            int len = lexer->pos - start;
            char *lex = malloc(len + 1);
            strncpy(lex, lexer->source + start, len);
            lex[len] = '\0';

            lexer->pos++;
            lexer->column++;

            Token *t = create_token(TOK_STRING, lex, lexer->line, start_column);
            TokenNode *n = malloc(sizeof(TokenNode));
            n->token = t;
            n->next = NULL;
            if (!head)
                head = tail = n;
            else
            {
                tail->next = n;
                tail = n;
            }
            continue;
        }

        if (strchr("=+-*/<>:{}(),!%;\"", lexer->source[lexer->pos]))
        {
            // Check for double operators like '==' or '<='
            char c = lexer->source[lexer->pos];
            char next = lexer->source[lexer->pos + 1] ? lexer->source[lexer->pos + 1] : '\0';
            if ((c == '=' || c == '<' || c == '>' || c == '!') && next == '=')
            {

                lexer->pos += 2;
                lexer->column += 2;
            }
            else
            {

                lexer->pos++;
                lexer->column++;
            }
        }
        else
        {
            // It's a word or number: stop at whitespace OR when hitting an operator
            while (lexer->source[lexer->pos] != '\0' &&
                   !isspace(lexer->source[lexer->pos]) &&
                   !strchr("=+-*/<>:{}(),!%;\"", lexer->source[lexer->pos]))
            {
                lexer->pos++;
                lexer->column++;
            }
        }

        int TOK_length = lexer->pos - start_pos;
        char *lexeme = malloc(TOK_length + 1);
        strncpy(lexeme, lexer->source + start_pos, TOK_length);
        lexeme[TOK_length] = '\0';

        TokenType type = Determine_Tokentype(lexeme);
        if (type == TOK_UNKNOWN)
        {
            fprintf(stderr, "Lexical Error: Unrecognized symbol '%s' at line %d, column %d\n",
                    lexeme, lexer->line, start_column);
            free(lexeme);
            continue;
        }
        Token *token = create_token(type, lexeme, lexer->line, start_column);

        // Create a linked list of tokens
        TokenNode *node = (TokenNode *)malloc(sizeof(TokenNode));
        node->token = token;
        node->next = NULL;

        if (!head)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }
    // Manually  appending EOF token to mark the end of input
    char *eof_lex = malloc(4);
    strcpy(eof_lex, "EOF");
    Token *eof_token = create_token(TOK_EOF, eof_lex, lexer->line, lexer->column);
    TokenNode *eof_node = (TokenNode *)malloc(sizeof(TokenNode));
    eof_node->token = eof_token;
    eof_node->next = NULL;
    if (!head)
        head = eof_node;
    else
        tail->next = eof_node;

    return head;
}

// Debugging purpose
const char *TokenType_To_String(TokenType type)
{
    switch (type)
    {
    case TOK_IF:
        return "TOK_IF";
    case TOK_EOF:
        return "TOK_EOF";
    case TOK_IDENTIFIER:
        return "TOK_IDENTIFIER";
    case TOK_INT:
        return "TOK_INT";
    case TOK_NUMBER:
        return "TOK_NUMBER";
    case TOK_BREAK:
        return "TOK_BREAK";
    case TOK_CONTINUE:
        return "TOK_CONTINUE";
    case TOK_PRINT:
        return "TOK_PRINT";
    case TOK_WHILE:
        return "TOK_WHILE";
    case TOK_RETURN:
        return "TOK_RETURN";
    case TOK_ASSIGN:
        return "TOK_ASSIGN";
    case TOK_PLUS:
        return "TOK_PLUS";
    case TOK_MINUS:
        return "TOK_MINUS";
    case TOK_STAR:
        return "TOK_STAR";
    case TOK_SLASH:
        return "TOK_SLASH";
    case TOK_LPAREN:
        return "TOK_LPAREN";
    case TOK_RPAREN:
        return "TOK_RPAREN";
    case TOK_SEMICOLON:
        return "TOK_SEMICOLON";
    case TOK_GREATER:
        return "TOK_GREATER";
    case TOK_LESS:
        return "TOK_LESS";
    case TOK_LEQUAL:
        return "TOK_LEQUAL";
    case TOK_GEQUAL:
        return "TOK_GEQUAL";
    case TOK_EQUALITY_CHECK:
        return "TOK_EQUALITY_CHECK";
    case TOK_LBRACE:
        return "TOK_LBRACE";
    case TOK_RBRACE:
        return "TOK_RBRACE";
    case TOK_COMMA:
        return "TOK_COMMA";
    case TOK_NOT:
        return "TOK_NOT";
    case TOK_MODULUS:
        return "TOK_MODULUS";
    case TOK_STRING:
        return "TOK_STRING";
    case TOK_UNKNOWN:
        return "TOK_UNKNOWN";
    default:
        return "INVALID_ENUM_VALUE"; 

        break;
    }
}
