#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

/* 
 * Retrieves a word from a file. Repeat until no words await reading
 * in the file.
 * @param s a character to be read in 
 * @param limit word check boundary
 * @param stream the file to read values from  
 * @return a word from the file 
 */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);

    while (!isalnum(c = getc(stream)) && EOF != c);

    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) {
        *w++ = tolower(c);
    }

    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }

    *w = '\0';
    return w - s;
}
/* 
 * Memory allocation function.
 * @param s the size of memory for malloc to allocate
 */
void *emalloc(size_t s) {
    void *out = malloc(s);

    if (NULL == out) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    return out;
}
/* 
 * Memory reallocation function. 
 * @param ptr a pointer for an object requiring reallocation 
 * @param s the size of memory for realloc to reallocate
 */
void *erealloc(void *ptr, size_t s) {
    void *out = realloc(ptr, s);

    if (NULL == out) {
        fprintf(stderr, "Memory reallocation failed!\n");
        exit(EXIT_FAILURE);
    }

    return out;
}
