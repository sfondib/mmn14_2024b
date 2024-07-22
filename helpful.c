#include "helpful.h"

/**
 * Allocates memory in required size, if process has been failed - exits the program
 * @param size - the size to allocate in bytes
 * @return a generic pointer to allocated memory
 */
void *malloc_check(long size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("Error: Memory allocation has failed, please try again.\n");
        exit(1);
    }
    return ptr;
}
