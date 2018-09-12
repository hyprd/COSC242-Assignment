#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "htable.h"
#include "tree.h"
#include "mylib.h"

typedef enum datastructure {TREE, HTABLE} datastructure_t;

/*
 * Checks if a number given by the user is a prime number.
 * @param n a number to check
 * @return an integer indicating whether or not the number is a prime number
 */
int is_prime(int n) {
    int i;

    if (n < 2) {
        return 0;
    }

    for (i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

/*
 * Returns the next highest prime greater than a given value.
 * @param i the base number
 * @return the next highest prime
 */
int next_highest_prime(int i) {
    int j;

    for (j = i; ; j++) {
        if (1 == is_prime(j)) {
            return j;
        }
    }
}

/* 
 * Print words added to the data structure alongside their frequencies.
 * @param frequency the frequency of a user-given word
 * @param word a word to print information about
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

/*
 * Generate a text block for message help within the terminal.
 */
static void print_help() {
    printf("HELP MESSAGE XD\n");
}

int main(int argc, char **argv) {
    const char *optstring = "Tc:deoprs:t:h";
    char option;
    datastructure_t datastructure = HTABLE;
    FILE *file_to_check = NULL;
    FILE *tree_view = NULL;
    hashing_t hashing_method = LINEAR_P;
    tree_t tree_type = BST;
    int htable_capacity = 113, snapshots = 10;

    /* Statements here represent command-line arguments with corresponding actions */
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'T':
                datastructure = TREE;
                break;
            case 'c':
                file_to_check = fopen(optarg, "r");

                if (file_to_check == NULL) {
                    fprintf(stderr, "Failed to open file: %s\n", optarg);
                    return EXIT_FAILURE;
                }
                
                break;
            case 'd':
                if (datastructure == HTABLE) {
                    hashing_method = DOUBLE_H;
                }
                break;
            case 'e':
                if (datastructure == HTABLE) {
                    htable_print_entire_table(h);
                }
                break;
            case 'o':
                if (file_to_check == NULL && datastructure == TREE) {
                    tree_view = fopen("tree-view.dot", "w");

                    if (tree_view == NULL) {
                        fprintf(stderr, "Failed to open file: %s\n", optarg);
                        return EXIT_FAILURE;
                    }
                }
                break;
            case 'p':
                if (file_to_check == NULL && datastructure == HTABLE) {
                    /* print-stats.txt*/
                }
                break;
            case 'r':
                if (datastructure == TREE) {
                    tree_type = RBT;
                }
                break;
            case 's':
                if (datastructure == HTABLE) {
                    snapshots = atoi(optarg);
                }
                break;
            case 't':
                if (datastructure == HTABLE) {
                    htable_capacity = next_highest_prime(atoi(optarg));
                }
                break;
            case 'h':
                print_help();
                break;
        }
    }
    /* Hash Table generation */
    if (datastructure = HTABLE) { 
        char *word;
        int unknown_words = 0, i;
        clock_t fill_start, fill_end, search_start, search_end;
        htable h = htable_new(htable_capacity, hashing_method);

        fill_start = clock();
        while (getword(word, sizeof word, stdin) != EOF) {
            htable_insert(h, word);
        }
        fill_end = clock();

        

        if (file_to_check != NULL) { /* -c filename AND NOT -o or -p */
            search_start = clock();
            while (getword(word, sizeof word, file_to_check) != EOF) {
                if (htable_search(h, word) == 0) {
                    printf("%s\n", word);
                    unknown_words++;
                }
            }
            search_end = clock();

            fprintf(stderr, "Fill time\t: %8.7f\n",
                    (fill_start - fill_end) / (double) CLOCKS_PER_SEC);
            fprintf(stderr, "Search time\t: %8.7f\n",
                    (search_start - search_end) / (double) CLOCKS_PER_SEC);
            fprintf(stderr, "Unknown words = %d\n", unknown_words);
        } else { /* NO -c filename so print normally */
            htable_print(h, print_info);
        }
    } else { /* TREES */
        
    }

    return EXIT_SUCCESS;
}
