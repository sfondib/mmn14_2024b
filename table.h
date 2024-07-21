/* Libraries*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/**
 * Add a new item to the table, in a sort way
 * @param tbl - table's pointer
 * @param value - the value of the new item
 * @param enter - the entry for the new value
 * @param type - the he type of the entry to insert
 */
void add_item_to_table(table *tbl, long value, char *enter, symbol_type type);

/**
 * Release all memory related to the table
 * @param tbl - the table to get free
 */
void free_table(table tbl);
