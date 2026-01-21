    #ifndef SYMBOL_TABLE_H
    #define SYMBOL_TABLE_H

    typedef struct {
        char name[32];
        int value;
    } Symbol;

    void symbol_table_init(void);

    #endif
