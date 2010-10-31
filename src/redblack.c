/* 
 * redblack.c
 * 
 * Copyright (c) 2010 Paul Osborne <osbpau@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * A red-black tree is a binary search tree where each node has a color 
 * attribute, the value of which is either red or black. In addition to the 
 * ordinary requirements imposed on binary search trees, the following 
 * additional requirements apply to red-black trees:
 * 1. A node is either red or black.
 * 2. The root is black. (This rule is used in some definitions and not others. 
 *    Since the root can always be changed from red to black but not necessarily 
 *    vice-versa this rule has little effect on analysis.)
 * 3. All leaves are black.
 * 4. Both children of every red node are black.
 * 5. Every simple path from a given node to any of its descendant leaves 
 *    contains the same number of black nodes.
 */

#include <stdint.h>
#include <malloc.h>
#include "redblack.h"

#define TEST_REDBLACK_INVARIANTS 0

/* We create a nil node in order to maintain red-black trees invariants */
static inline RedblackNode* redblack_create_nil() {
    RedblackNode* nil = malloc(sizeof(RedblackNode));
    nil->left = NULL;
    nil->right = NULL;
    nil->color = REDBLACK_BLACK; /* nil nodes are always black */
    nil->value = NULL;
    return nil;
}


/* Allocate a redblack node (all new nodes in tree start red) */
static inline RedblackNode* redblack_create_node(void *value) {
    RedblackNode *node = malloc(sizeof(struct redblack_node));
    node->left = NULL;
    node->right = NULL;
    node->color = REDBLACK_RED;
    return node;
}

#if 0
/*
 * Perform single or double rotation on the tree to put it in good state
 *
 * Because the result is attached to the parent, there are 4 cases.
 */
static inline RedblackNode* redblack_rotate(RedblackTree *tree, void *item, RedblackNode *parent,
                                            RedblackNode *grandparent, RedblackNode *great_grandparent) {
    int8_t tmp;
    RedblackNode* pivot_node;
    if (tree->compare_func(item, parent->value) > 0) {
        tmp = tree->compare_func(item, parent->left);
        if (tmp < 0) {
            parent->left = redblack_rotate_with_left_child(tree, parent->left);
        } else {
            parent->left = redblack_rotate_with_right_child(tree, parent->left);
        }
        pivot_node = parent->left;
    } else {
        tmp = tree->compare_func(tree, parent->right);
        if (tmp < 0) {
            parent->right = redblack_rotate_with_left_child(tree, parent->right);
        } else {
            parent->right = redblack_rotate_with_right_child(tree, parent->right);
        }
        pivot_node = parent->right;
    }
    return pivot_node;
}


/* Internal inline function for perform reorientation of tree when an insertion
 * is performed and some node has two red children.  In this case we must
 * performs flips and rotations on the tree.
 *
 * We pass in temporary state used during insertion rather than storing
 * it with the tree to save on space.
 */
static inline void redblack_handle_reorient(RedblackTree *tree, void *item,
                                            RedblackNode *current, RedblackNode *parent,
                                            RedblackNode *grandparent, RedblackNode *great_grandparent) {
    current->color = REDBLACK_RED;
    current->left->color = REDBLACK_BLACK;
    current->right->color = REDBLACK_BLACK;

    if (parent->color == REDBLACK_RED) {
        grandparent->color = REDBLACK_RED;
        if (tree->compare_func(item, grandparent->value) < 0) {
            parent = redblack_rotate(tree, item, parent, grandparent, great_grandparent);
        }
        current = redblack_rotate(tree, item, parent, grandparent, great_grandparent);
        current->color = REDBLACK_BLACK;
    }
    tree->root->color = REDBLACK_BLACK; /* INVARIANT: root always black */
}
#endif
/* Create an empty redblack tree with the specified comparison function
 * 
 * A reference to the newly created tree will be returned.  If memory cannot
 * be allocated, NULL will be returned.
 */
RedblackTree*
redblack_create(int8_t(*compare_func)(const void*, const void*)) {
    RedblackTree* tree;
    tree = malloc(sizeof(struct redblack_node));
    if (tree == NULL) {
        return NULL;
    }

    tree->nil = redblack_create_nil(); /* create nil for this tree */
    tree->root = tree->nil; /* start out with root as nil */
    tree->compare_func = compare_func;
    return tree;
}

#if 0
/* Insert the specified item into the red-black tree in the correct place.
 * 
 * 
 */
void
redblack_insert(RedblackTree* tree, void* item) {
    /* everything starts pointing to the root */
    uint8_t comparison_val;
    RedblackNode *current, *parent, *grandparent, *great_grandparent;
    current = parent = grandparent = great_grandparent = tree->root;

    /* store the item */
    while ((comparison_val = tree->compare_func(item, current->value)) != 0) {
        great_grandparent = grandparent; grandparent = parent; parent = current;
        current = comparison_val < 0 ? current->left : current->right;

        /* if there are two red children we need to fix something */
        if (current->left->color == REDBLACK_RED && current->right->color == REDBLACK_RED) {
            redblack_handle_reorient(tree, item, current, parent, grandparent, great_grandparent);
        }
    }

    /* Allocate memory for the new node */
    current = redblack_create_node(item);

    /* Attach the node to its parent */
    if (tree->compare_func(item, parent->value) < 0) {
        parent->left = current;
    } else {
        parent->right = current;
    }

    redblack_handle_reorient(tree, item, current, parent, grandparent, great_grandparent);
}
#endif
/* 
 * 
 */
void*
redblack_remove(RedblackTree* tree, void* item) {
    return NULL;
}

/* Return TRUE (1) if the tree contains a node equal to the specified item
 * and FALSE (0) if not.
 */
int8_t
redblack_contains(RedblackTree* tree, void* item) {
    return 0; /* TODO */
}

/* 
 * 
 */
RedblackIterator*
redblack_iterator_create(RedblackTree* tree, uint8_t iteration_order) {
    return NULL; /* TODO */
}

/* 
 * 
 */
void redblack_iterator_free(RedblackIterator* iterator) {
    return NULL; /* TODO */
}

/* 
 * 
 */
void
redblack_iterator_next(RedblackIterator* iterator) {

}

/* 
 * 
 */
int8_t redblack_iterator_hasnext(RedblackIterator* iterator) {
    return 0;
}

#if 0
#include <stdio.h>
int main() {
    printf("Starting some simple tests\r\n");
    RedblackTree *tree = redblack_create(NULL);
    return 0;
}
#endif
