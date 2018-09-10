#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

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

void *emalloc(size_t s) {
    void *out = malloc(s);

    if (NULL == out) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    return out;
}

void *erealloc(void *ptr, size_t s) {
    void *out = realloc(ptr, s);

    if (NULL == out) {
        fprintf(stderr, "Memory reallocation failed!\n");
        exit(EXIT_FAILURE);
    }

    return out;
}
