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
 * Rotate the nodes of a RBT tree to the left.
 * @param t a given tree to rotate
 * @return the updated tree
 */
tree left_rotate(tree t)
{
    tree tmp = t;
    t = t->right;
    tmp->right = t->left;
    t->left = tmp;
    return t;
}
/* 
 * Rotate the nodes of a RBT tree to the right.
 * @param t a given tree to rotate
 * @return the updated tree
 */
tree right_rotate(tree t)
{
    tree tmp = t;
    t = t->left;
    tmp->left = t->right;
    t->right = tmp;
    return t;
}

/*
 * Create a new tree.
 * @param type used to define what tree the program creates
 * @return the newly created tree with a user-defined type
 */
tree tree_new(tree_t type)
{
    
    tree_type = type;
    return NULL;    
}

/*
 * Flip the colours of the tree so that children are 
 * BLACK with RED roots.
 * @param t a given tree to flip colours
 * @return return a post-flip tree
 */
static tree flipColour(tree t){
    t->colour = RED;
    t->left->colour = BLACK;
    t->right->colour = BLACK;
    return t;
}

/* 
 * Fix trees when their structure is altered.
 * @param t a given tree requiring restructuring
 * @return the post-fix tree
 */
tree tree_fix(tree b){

    if (IS_RED(b->left) && IS_RED(b->left->left) && IS_RED(b->right)){
        b = flipColour(b);
    }
    if(IS_RED(b->left) && IS_RED(b->left->left) && IS_BLACK(b->right)){
        b = right_rotate(b);
        b->colour = BLACK;
        b->left->colour = RED;
        b->right->colour = RED;
    }
    if(IS_RED(b->left) && IS_RED(b->left->right) && IS_RED(b->right)) {
        b = flipColour(b);
    }
    if(IS_RED(b->left) && IS_RED(b->left->right) && IS_BLACK(b->right)) {
        b->left = left_rotate(b->left);
        b = right_rotate(b);
        b->colour = BLACK;
        b->left->colour = RED;
        b->right->colour = RED;
    }
    if(IS_RED(b->right) && IS_RED(b->right->left) && IS_RED(b->left)) {
        b = flipColour(b);
    }
    if(IS_RED(b->right) && IS_RED(b->right->left) && IS_BLACK(b->left)) {
        b->right = right_rotate(b->right);
        b = left_rotate(b);
        b->colour = BLACK;
        b->left->colour = RED;
        b->right->colour = RED;
    }
    if(IS_RED(b->right) && IS_RED(b->right->right) && IS_RED(b->left)) {
        b = flipColour(b);
    }
    if(IS_RED(b->right) && IS_RED(b->right->right) && IS_BLACK(b->left)) {
        ;
        b = left_rotate(b);
        b->colour = BLACK;
        b->left->colour = RED;
        b->right->colour = RED;
    }
    return b;
    
}

/* 
 * Insert a value into a given tree.
 * @param t a given tree to insert a value into
 * @param str the value to be inserted into the tree
 */
tree tree_insert (tree t, char *str) {
    if (t == NULL){
        
        tree t = emalloc(sizeof *t);
        t->key = emalloc((strlen(str) +1) * sizeof(char));
        
        strcpy(t->key, str);
        t->frequency++;
        t->left = NULL;
        t->right = NULL;
        t->colour = RED;
        if (tree_type == RBT) {
            t = tree_fix(t);
        }
        return t;
        
    }
    
    else if (strcmp(t->key, str) == 0){
        t->frequency++;
        return t;
    }
    else if (strcmp(str, t->key) > 0){
        t->right = tree_insert(t->right, str);
    }
    else{
        t->left = tree_insert(t->left, str);
    }
    
    if(tree_type == RBT)
        {
            t = tree_fix(t);
        }
    return t;
}

/*
 * Search a given tree for a value given by the user. Return 1 if the value
 * is found, 0 otherwise
 * @param b a given tree to search for values
 * @param str the value to search the tree for
 * @return an integer to indicate search results. 1 for success, 0 for failure.
 *  Return a recursive function call if keys don't match
 */
int tree_search(tree t, char *str) {
    if (t == NULL) {
        return 0;
    }
    if (strcmp(str, t->key) == 0) {
        return 1;
    } else if (strcmp(str, t->key) < 0) {
        return tree_search(t->left, str);
    } else {
        return tree_search(t->right, str);
    }
}

/* 
 * Traverse a tree in in-order fashion.
 * @param b a given tree to traverse
 * @param f a function given a value to traverse
 */
void tree_inorder(tree t, void f(char *str)) {
    if (t == NULL) {
        return;
    }
    tree_inorder(t->left, f);
    f(t->key);
    tree_inorder(t->right, f);
}

/* 
 * Traverse a tree in pre-order fashion.
 * @param b a given tree to traverse
 * @param f a function given a word frequency value aswell as a key value to traverse
 */
void tree_preorder(tree t, void f(int frequency, char *str)) {
    if (t == NULL) {
        return;
    }
    f(t->frequency, t->key);
    tree_preorder(t->left, f);
    tree_preorder(t->right, f);
}


/* 
 * Free the memory allocated to a given tree.
 * @param b a given tree to free memory from
 * @return a tree freed from memory
 */
tree tree_free(tree t) {
    if (t == NULL){
        return t;
    }else{
        tree_free(t->left);
        tree_free(t->right);
        free(t->key);
        free(t);
        return t;
    }
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
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
