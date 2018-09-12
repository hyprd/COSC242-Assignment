#include "tree.h"
#include <string.h>
#include <stdlib.h>
#include "mylib.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

static tree_t tree_type;

/* Generate tree struct */
struct tree_node {
    char *key;
    tree_colour colour;
    tree left;
    tree right;
    int frequency;
};

/* 
 * Rotate the nodes of a tree to the left.
 * @param b a given tree to rotate
 * @return the updated tree
 */
static tree left_rotate(tree b) {    
    tree root, temp;

    root = b;
    temp = root;

    root = b->right;
    temp->right = root->left;

    root->left = temp;

    return root;
}

/* 
 * Rotate the nodes of a tree to the right.
 * @param b a given tree to rotate
 * @return the updated tree
 */
static tree right_rotate(tree b) {
    tree root, temp;

    root = b;
    temp = root;

    root = b->left;
    temp->left = root->right;

    root->right = temp;

    return root;
}

/*
 * Create a new tree.
 * @param type used to define what tree the program creates
 * @return the newly created tree with a user-defined type
 */
tree tree_new(tree_t type) {
    tree b = emalloc(sizeof(struct tree_node));
    b->key = NULL;
    b->left = NULL;
    b->right = NULL;
    tree_type = type;

    b->colour = RED;
    b->frequency = 0;
    
    return b;
}

/* 
 * Fix trees when their structure is altered.
 * @param b a given tree requiring restructuring
 * @return the post-fix tree
 */
tree tree_fix(tree b){
    if(IS_RED(b->left) && IS_RED(b->left->left)){
        if (IS_RED(b->right)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b = right_rotate(b);
            b->colour = BLACK;
            b->right->colour = RED;
        }
    }

    else if(IS_RED(b->left) && IS_RED(b->left->right)){
        if (IS_RED(b->right)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b->left = left_rotate(b->left);
            b = right_rotate(b);
            b->colour = BLACK;
            b->right->colour = RED;
        }
    }

    else if(IS_RED(b->right) && IS_RED(b->right->left)){
        if (IS_RED(b->left)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b->right = right_rotate(b->right);
            b = left_rotate(b);
            b->colour = BLACK;
            b->right->colour= RED;
        }
    }

    else if (IS_RED(b->right) && IS_RED(b->right->right)){
        if (IS_RED(b->left)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b = left_rotate(b);
            b->colour = BLACK;
            b->left->colour = RED;
        }
    }

    return b;
}

/* 
 * Insert a value into a given tree.
 * @param b a given tree to insert a value into
 * @param str the value to be inserted into the tree
 */
tree tree_insert(tree b, char *str) {
    int cmp;
    if (b == NULL) {
        b = tree_new(tree_type);
    }
    
    if (b->key == NULL) {
        b->key = emalloc(sizeof(char) * (strlen(str) + 1));
        strcpy(b->key, str);
        b->frequency = 1;
        return b;
    }
    
    cmp = strcmp(str, b->key);
    if (cmp < 0) {
        b->left = tree_insert(b->left, str);
    } else if (cmp > 0) {
        b->right = tree_insert(b->right, str);
    } else if (cmp == 0) {
        b->frequency += 1;
    }
    
    if (tree_type == RBT) {
        b = tree_fix(b);
    }
    
    return b;
}

/*
 * Search a given tree for a value given by the user. Return 1 if the value
 * is found, 0 otherwise
 * @param b a given tree to search for values
 * @param str the value to search the tree for
 */
int tree_search(tree b, char *str) {
    if (b == NULL) {
        return 0;
    }
    if (strcmp(str, b->key) == 0) {
        return 1;
    } else if (strcmp(str, b->key) < 0) {
        return tree_search(b->left, str);
    } else {
        return tree_search(b->right, str);
    }
}

/* 
 * Traverse a tree in in-order fashion.
 * @param b a given tree to traverse
 * @param f a function given a value to traverse
 */
void tree_inorder(tree b, void f(char *str)) {
    if (b == NULL) {
        return;
    }
    tree_inorder(b->left, f);
    f(b->key);
    tree_inorder(b->right, f);
}
/* 
 * Traverse a tree in pre-order fashion.
 * @param b a given tree to traverse
 * @param f a function given a word frequency value aswell as a key value to traverse
 */
void tree_preorder(tree b, void f(int frequency, char *str)) {
    if (b == NULL) {
        return;
    }
    f(b->frequency, b->key);
    tree_preorder(b->left, f);
    tree_preorder(b->right, f);
}

/*
 * Set the colour of the tree to BLACK.
 * @param b a given tree to change colour
 * @return a post-coloured tree
 */
tree setColourBlack(tree b) {
    b->colour = BLACK;
    return b;
}

/* 
 * Free the memory allocated to a given tree.
 * @param b a given tree to free memory from
 * @return a tree freed from memory
 */
tree tree_free(tree b) {
    if (b == NULL){
        return b;
    }else{
        tree_free(b->left);
        tree_free(b->right);
        free(b->key);
        free(b);
        return b;
    }
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}
