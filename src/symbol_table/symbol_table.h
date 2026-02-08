#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stddef.h>


typedef enum {
    TYPE_INT,
    TYPE_ERROR
} DataType;

typedef struct Symbol {
    char name[32];
    DataType type;
    int scope_level;
    struct Symbol* next;
} Symbol;

/* Scope control */
void enter_scope(void);
void exit_scope(void);

/* Symbol operations */
void insert_symbol(const char* name, DataType type);
Symbol* lookup_symbol(const char* name);
Symbol* lookup_current_scope(const char* name);

void print_symbol_table(void);

/* Cleanup */
void free_symbol_table(void);

#endif
