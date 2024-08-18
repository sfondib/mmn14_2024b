#include "helpful.h"
#include "symbol_table.h"

void addItemToTable(table *tbl, long value, char *item, symbol_type type) {
    char *temp_item;
    table prev_item, curr_item, new_item;
    
    /* Allocate memory for new and 'temp' items */
    new_item = (table) mallocCheck(sizeof(table_item));
    temp_item = (char *) mallocCheck(strlen(item) + 1);
    strcpy(temp_item, item);
    
    /* Fix the member name from item to key */
    new_item->key = temp_item;
    new_item->value = value;
    new_item->type = type;
  
    /* If table is empty, add the new item to the head of the table, or if new item's value is smallest then the current head */
    if ((*tbl) == NULL || (*tbl)->value > value) {
        new_item->next = (*tbl);
        (*tbl) = new_item;
        return;
    }

    /* Insert the new table entry, keeping it sorted */
    curr_item = (*tbl)->next;
    prev_item = *tbl;
    while (curr_item != NULL && curr_item->value < value) {
        prev_item = curr_item;
        curr_item = curr_item->next;
    }

    new_item->next = curr_item;
    prev_item->next = new_item;
}

void freeTableMemory(table *tbl) {
    table prev_item, curr_item = *tbl;
    while (curr_item != NULL) {
        prev_item = curr_item;
        curr_item = curr_item->next;
        free(prev_item->key);
        free(prev_item);
    }
    *tbl = NULL;  /* Make sure the table pointer is set to NULL after freeing */
}

void addValueIfTypeMatch(table tbl, long to_add, symbol_type type) {
    /* Go over each item in the table and add the value to_add if the type matches */
    while (tbl != NULL) {
        if (tbl->type == type) {
            tbl->value += to_add;
        }
        tbl = tbl->next;
    }
}

table_item *findType(table tbl, char *key, int symbol_count, ...) {
    va_list arglist;  /* Variable argument list to handle multiple symbol types */

    if (tbl == NULL) { /* Check if table is NULL */
        return NULL;
    }

    va_start(arglist, symbol_count);  /* Initialize the argument list */

    /* Iterate over the table entries */
    do {
        /* Check if the current table item matches the key and one of the valid symbol types */
        if (isTypeMatching(tbl, key, arglist, symbol_count)) {
            va_end(arglist);  /* Clean up the argument list before returning */
            return tbl;  /* Return the matching table item */
        }
        va_end(arglist);  /* End the current argument list */
        va_start(arglist, symbol_count);  /* Reinitialize the argument list for the next iteration */
    } while ((tbl = tbl->next) != NULL);  /* Move to the next table entry */

    va_end(arglist);  /* Clean up the argument list after the loop */
    return NULL;  /* No match found, return NULL */
}
