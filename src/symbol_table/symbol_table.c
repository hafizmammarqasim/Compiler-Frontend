#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symbol_table.h"

static Symbol* symbol_table = NULL;
static int current_scope = 0;

/* Enter a new scope */
void enter_scope(void) {
    current_scope++;
}

/* Exit current scope and delete its symbols */
void exit_scope(void) {
    if (current_scope <= 0) {
        fprintf(stderr, "Warning: exit_scope called at global scope; no scope to exit\n");
        return;
    }

    Symbol* temp = symbol_table;
    Symbol* prev = NULL;

    while (temp) {
        if (temp->scope_level == current_scope) {
            if (prev)
                prev->next = temp->next;
            else
                symbol_table = temp->next;

            Symbol* to_delete = temp;
            temp = temp->next;
            free(to_delete);
        } else {
            prev = temp;
            temp = temp->next;
        }
    }

    current_scope--;
}

/* Insert variable into symbol table */
void insert_symbol(const char* name, DataType type) {
    if (!name) {
        fprintf(stderr, "Semantic Error: insert_symbol called with NULL name\n");
        return;
    }

    if (lookup_current_scope(name)) {
        fprintf(stderr, "Semantic Error: Variable '%s' redeclared in same scope\n", name);
        return;
    }

    Symbol* sym = (Symbol*)malloc(sizeof(Symbol));
    if (!sym) {
        fprintf(stderr, "Internal Error: Memory allocation failed while inserting symbol '%s'\n", name);
        return;
    }

    /* Ensure name fits in buffer (reserve 1 byte for NUL) */
    size_t maxlen = sizeof(sym->name) - 1;
    size_t namelen = strlen(name);
    if (namelen > maxlen) {
        fprintf(stderr, "Semantic Error: Variable name '%s' too long (max %zu chars)\n", name, maxlen);
        free(sym);
        return;
    }

    /* Safe copy and null-terminate */
    memcpy(sym->name, name, namelen + 1);

    sym->type = type;
    sym->scope_level = current_scope;
    sym->next = symbol_table;

    symbol_table = sym;
}

/* Lookup variable in current scope only */
Symbol* lookup_current_scope(const char* name) {
    if (!name) return NULL;
    Symbol* temp = symbol_table;
    while (temp) {
        if (strcmp(temp->name, name) == 0 &&
            temp->scope_level == current_scope)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/* Lookup variable with scope resolution */
Symbol* lookup_symbol(const char* name) {
    if (!name) return NULL;
    Symbol* temp = symbol_table;
    Symbol* best = NULL;
    int max_scope = -1;

    while (temp) {
        if (strcmp(temp->name, name) == 0 &&
            temp->scope_level <= current_scope &&
            temp->scope_level > max_scope) {
            best = temp;
            max_scope = temp->scope_level;
        }
        temp = temp->next;
    }
    return best;
}

/* Print symbol table */
void print_symbol_table(void) {
    Symbol* temp = symbol_table;
    printf("\n--- SYMBOL TABLE ---\n");
    printf("Name\tType\tScope\n");

    if (!temp) {
        printf("<empty>\n");
        return;
    }

    while (temp) {
        printf("%s\t%s\t%d\n",
            temp->name,
            temp->type == TYPE_INT ? "INT" : "ERROR",
            temp->scope_level);
        temp = temp->next;
    }
}

/* Free entire symbol table (call at program exit) */
void free_symbol_table(void) {
    Symbol* temp = symbol_table;
    while (temp) {
        Symbol* next = temp->next;
        free(temp);
        temp = next;
    }
    symbol_table = NULL;
    current_scope = 0;
}
