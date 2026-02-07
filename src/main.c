/*
#include <stdio.h>
#include "symbol_table/symbol_table.h"

int main() {
    insert_symbol("x", TYPE_INT);

    enter_scope();
     // shadowing allowed
    insert_symbol("y", TYPE_INT);

    Symbol* s = lookup_symbol("x");
    if (s)
        printf("Found x: type=%d scope=%d\n", s->type, s->scope_level);

    exit_scope();

    if (!lookup_symbol("y"))
        printf("y is out of scope\n");

    print_symbol_table();

    /* cleanup */
   // free_symbol_table();

    return 0;
}*/