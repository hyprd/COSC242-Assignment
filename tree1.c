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

/**
 * Used by red black trees to rotate the tree to the left.
 * @param t tree to rotate.
 * @return rotated tree.
 */
tree left_rotate(tree t)
{
    tree tmp = t;
    t = t->right;
    tmp->right = t->left;
    t->left = tmp;
    return t;
}
/**
 * Used by red black trees to rotate the tree to the right.
 * @param t tree to rotate.
 * @return rotated tree.
 */
tree right_rotate(tree t)
{
    tree tmp = t;
    t = t->left;
    tmp->left = t->right;
    t->right = tmp;
    return t;
}

tree tree_new(tree_t type) {
    /* need to find correct code*/
}


tree tree_fix(tree t){

    if(IS_RED(t->left) && IS_RED(t->left->left)){
        if (IS_RED(t->right)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }else{
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    }

    else if(IS_RED(t->left) && IS_RED(t->left->right)){
        if (IS_RED(t->right)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }else{
            t->left = left_rotate(t->left);
            t = right_rotate(t);
            t->colour = BLACK;
            t->right->colour = RED;
        }
    }

    else if(IS_RED(t->right) && IS_RED(t->right->left)){
        if (IS_RED(t->left)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }else{
            t->right = right_rotate(t->right);
            t = left_rotate(t);
            t->colour = BLACK;
            t->right->colour= RED;
        }
    }

    else if (IS_RED(t->right) && IS_RED(t->right->right)){
        if (IS_RED(t->left)){
            t->colour = RED;
            t->left->colour = BLACK;
            t->right->colour = BLACK;
        }else{
            t = left_rotate(t);
            t->colour = BLACK;
            t->left->colour = RED;
        }
    }

    return t;
}

tree tree_insert (tree t, char *str) {
    /* needs to be fixed */
}

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

void tree_inorder(tree t, void f(char *str)) {
    if (t == NULL) {
        return;
`    }
    tree_inorder(t->left, f);
    f(t->key);
    tree_inorder(t->right, f);
}

void tree_preorder(tree t, void f(int frequency, char *str)) {
    if (t == NULL) {
        return;
    }
    f(t->frequency, t->key);
    tree_preorder(t->left, f);
    tree_preorder(t->right, f);
}

/*tree setColourBlack(tree t) {
  t->colour = BLACK;
  return t;
  }*/

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
