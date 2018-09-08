#include "mylib.h"
#include <stdio.h>
#include <stdlib.h>



/**
method which help to mallocate
*/
void *emalloc(size_t s) {
void *result = malloc(s);
if (result == NULL) {
fprintf(stderr, "memory allocation failed");
exit(EXIT_FAILURE);
}
return result;

}

/**
method reallocate given by a pointer and the size
*/
void *erealloc(void *p, size_t s) {
void *result = realloc(p, s);
if (result == NULL) {
fprintf(stderr, "memory allocation failed");
exit(EXIT_FAILURE);
}
return result;
}
