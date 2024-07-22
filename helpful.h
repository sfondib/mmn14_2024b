/* in helpul file we will find helpful functions that will take part in the whole program */
#ifndef HELPFUL_H
#define HELPFUL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Allocates memory in required size, if process has been failed - exits the program
 * @param size - the size to allocate in bytes
 * @return a generic pointer to allocated memory
 */
void *malloc_check(long size);

#endif /* HELPFUL_H */
