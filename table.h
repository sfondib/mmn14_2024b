#ifndef TABLE_H
#define TABLE_H
/* Libraries*/
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
void add_item_to_table(table *tbl, long value, char *item, symbol_type type);

/**
 * Release all memory related to the table
 * @param tbl - the table to be freed
 */
void free_table(table tbl);

#endif /* TABLE_H */
