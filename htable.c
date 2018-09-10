#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

struct htablerec {
    char **keys;
    int *frequencies;
    int num_keys;
    int capacity;
    int *stats;
    hashing_t method;
};

static unsigned int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity - 1));
}

static unsigned int str_to_int(char *str) {
    unsigned int out = 0;

    while (*str != '\0') {
        out = (*str++ + 31 * out);
    }

    return out;
}

htable htable_new(int capacity, hashing_t method) {
    int i;

    htable h = emalloc(sizeof *h);

    h->capacity = capacity;
    h->num_keys = 0;

    h->frequencies = emalloc(h->capacity * sizeof h->frequencies[0]);
    h->keys = emalloc(h->capacity * sizeof h->keys[0]);
    h->stats = emalloc(h->capacity * sizeof h->stats[0]);

    h->method = method;

    for (i = 0; i < h->capacity; i++) {
        h->frequencies[i] = 0;
        h->keys[i] = NULL;
    }

    return h;
}

void htable_free(htable h) {
    int i;

    for (i = 0; i < h->capacity; i++) {
        free(h->keys[i]);
    }

    free(h->keys);
    free(h->frequencies);

    free(h);
}

int htable_insert(htable h, char *str) {
    unsigned int index = str_to_int(str) % h->capacity;
    unsigned int index_origin;
    unsigned int step;
    int collisions = 0;

    if (h->method == DOUBLE_H) {
        step = htable_step(h, index);
    } else {
        step = 1;
    }

    if (h->keys[index] == NULL) {
        h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0][0]);
        strcpy(h->keys[index], str);

        h->frequencies[index] = 1;
        h->stats[h->num_keys] = collisions;
        h->num_keys++;

        return 1;
    }

    if (strcmp(h->keys[index], str) == 0) {
        return ++h->frequencies[index];
    }

    index_origin = index;

    do {
        index += step;
        index %= h->capacity;
        collisions++;

        if (h->keys[index] == NULL) {
            h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0][0]);
            strcpy(h->keys[index], str);

            h->frequencies[index] = 1;
            h->stats[h->num_keys] = collisions;
            h->num_keys++;

            return 1;
        }

        if (strcmp(h->keys[index], str) == 0) {
            return ++h->frequencies[index];
        }
    } while (index != index_origin);

    return 0;
}

void htable_print(htable h, void f(int freq, char *key)) {
    int i;

    for (i = 0; i < h->capacity; i++) {
        if (h->frequencies[i] > 0) {
            f(h->frequencies[i], h->keys[i]);
        }
    }
}

void htable_print_entire_table(htable h, FILE *stream) {
    int i;

    fprintf(stream, "  Pos  Freq  Stats  Word\n");
    fprintf(stream, "----------------------------------------\n");

    for (i = 0; i < h->capacity; i++) {
        fprintf(stream, "%5d %5d %5d   %s\n",
            i, h->frequencies[i], h->stats[i], h->keys[i]);
    }
}

int htable_search(htable h, char *str) {
    int collisions = 0;
    unsigned int index = str_to_int(str) % h->capacity;
    unsigned int step;

    if (h->method == DOUBLE_H) {
        step = htable_step(h, index);
    } else {
        step = 1;
    }

    while (NULL != h->keys[index]
           && strcmp(h->keys[index], str) != 0
           && collisions < h->capacity) {
        index += step;
        index %= h->capacity;
        collisions++;
    }

    if (collisions == h->capacity) {
        return 0;
    } else {
        return h->frequencies[index];
    }
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            } 
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
    
        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full, 
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n", 
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}
