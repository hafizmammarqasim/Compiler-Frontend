#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

typedef struct Symbol {
    char *name;         // Variable name 
    char *type;         // Variable type 
    int line_declared; 
    struct Symbol *next;
} Symbol;

// Functions
void symbol_table_init();
bool symbol_add(const char *name, const char *type, int line);
Symbol* symbol_lookup(const char *name);
void print_symbol_table();
void free_symbol_table();

#endif