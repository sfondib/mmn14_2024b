/* Libraries*/
#include "helpful.h"
#include "table.h"									 

void add_item_to_table(table *tbl, long value, char *item, symbol_type type) {
    char *temp_item;
    table prev_item, curr_item, new_item;
    
    /* allocate memory for new and 'temp' items */
    new_item = (table) malloc_check(sizeof(table_item));
    temp_item = (char *) malloc_check(strlen(item) + 1);
    strcpy(temp_item, item);
    
    /* Fix the member name from item to key */
    new_item->key = temp_item;
    new_item->value = value;
    new_item->type = type;
  
    /* if table is empty, add the new item to the head of the table, or if new item's value is smallest then the current head */
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

void add_value_if_type_match(table tbl, long to_add, symbol_type type) {
    /* Go over each item in the table and add the value to_add if the type matches */
    while (tbl != NULL) {
        if (tbl->type == type) {
            tbl->value += to_add;
        }
        tbl = tbl->next;
    }
}

void free_table_memory(table tbl) {
    table prev_item, curr_item = tbl;
    while (curr_item != NULL) {
        prev_item = curr_item;
        curr_item = curr_item->next;
        free(prev_item->key);
        free(prev_item);
    }
}
