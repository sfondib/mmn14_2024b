#include "helpful.h"

void *malloc_check(long size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("Error: Memory allocation has failed, please try again.\n");
        exit(1);
    }
    return ptr;
}
