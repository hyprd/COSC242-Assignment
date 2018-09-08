#include "tree.h"
#include <string.h>
#include <stdlib.h>
#include "mylib.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

static tree_t tree_type;

struct tree_node {
    char *key;
    tree_colour colour;
    tree left;
    tree right;
    int frequency;
};

static tree left_rotate(tree b) {    
    tree root, temp;

    root = b;
    temp = root;

    root = b->right;
    temp->right = root->left;

    root->left = temp;

    return root;
}

static tree right_rotate(tree b) {
    tree root, temp;

    root = b;
    temp = root;

    root = b->left;
    temp->left = root->right;

    root->right = temp;

    return root;
}


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
 * returns 0 if not found
 * returns 1 if found
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

void tree_inorder(tree b, void f(char *str)) {
    if (b == NULL) {
        return;
    }
    tree_inorder(b->left, f);
    f(b->key);
    tree_inorder(b->right, f);
}

void tree_preorder(tree b, void f(int frequency, char *str)) {
    if (b == NULL) {
        return;
    }
    f(b->frequency, b->key);
    tree_preorder(b->left, f);
    tree_preorder(b->right, f);
}

tree setColourBlack(tree r) {
    r->colour = BLACK;
    return r;
}

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
