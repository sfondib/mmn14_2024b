#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Pointer to table's entry */
typedef struct item* table;

/* Symbol type of item in the table */
typedef enum symbol_type {
    CODE_SYMBOL,
    DATA_SYMBOL,
    EXTERNAL_SYMBOL,
    EXTERNAL_REFERENCE,
    ENTRY_SYMBOL
} symbol_type;

/* Table item */
typedef struct item {
    table next; /* Next item */
    long value; /* Item's Address */
    char *key; /* item name (a string) */
    symbol_type type; /* Symbol type */
} table_item;

/**
 * Add a new item to the table, in a sorted way
 * @param tbl - table's pointer
 * @param value - the value of the new item
 * @param item - the entry for the new value
 * @param type - the type of the entry to insert
 */
void addItemToTable(table *tbl, long value, char *item, symbol_type type);

/**
 * Adds the value to add into the value of each entry
 * @param tbl - table
 * @param to_add - the value to add if type and value matches
 * @param type - the type of symbols that should be add
 */
void addValueIfTypeMatch(table tab, long to_add, symbol_type type);

/**
 * Release all memory related to the table
 * @param tbl - the table to be freed
 */
void freeTableMemory(table tbl);

/**
 * Find an item in the table by its key and a list of valid types
 * @param tbl - the item to search
 * @param key - the key to match
 * @param symbol_count - the number of valid symbol types to check
 * @param ... - variadic list of symbol types to check against
 * @return a pointer to the matching entry, or NULL if not found
 */
table_item *findType(table tbl, char *key, int symbol_count, ...);

#endif /* SYMBOL_TABLE_H */
