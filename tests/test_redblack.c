/*
 * test_deque.c
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
 */

#include <check.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "../src/arraylist.h"
#include "../src/redblack.h"

#define FALSE 0
#define TRUE  1

uint8_t redblack_tree_valid(RedblackTree *tree);
uint8_t redblack_validate_node(RedblackTree *tree, RedblackNode *node);


int8_t integer_comparator(void* a, void* b) {
    return *(int*)a - *(int*)b;
}

void redblack_path_to_root(RedblackTree *tree, ArrayList list, RedblackNode *node) {
    RedblackNode* current = tree->root;
    while (current != node) {
        arraylist_append(list, current);
        if (tree->compare_func(current->value, node->value) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return;
}

uint8_t redblack_validate_node(RedblackTree *tree, RedblackNode* node) {
    ArrayList parents = arraylist_create(integer_comparator);

    /* if we are checking the nil node, assume we are good */
    if (node == tree->nil) {
        return TRUE;
    }

    /* check if we are a leaf node, if so we must be black */
    if (node->left == tree->nil && node->right == tree->nil && node->color == REDBLACK_RED) {
        return FALSE;
    }

    /* if we are a red node we must have two valid children and they must
     * both be black */
    if (node->color == REDBLACK_RED) {
        if (node->left == tree->nil ||
            node->right == tree->nil ||
            node->left->color != REDBLACK_BLACK ||
            node->right->color != REDBLACK_BLACK) {
            return FALSE;
        }
    }

    /* Now the tricky one, every "simple path" from a given node to any of
     * its descendants contains the same number of black nodes
     *
     * Here we jump through some hoops.  From where we are we have to trace
     * back to the root.  Unfortunately, we don't have any information stored
     * about our parent (to save on memory).  We have to examine the tree in
     * order to find out where we are in the tree.
     */

}

/*
 * Validate the tree against the five criterion for a redblack tree:
 *
 * 1) A node is either red or black
 * 2) The root is black
 * 3) All leaves are black
 * 4) Both children of every red node are black
 * 5) Every "simple path" from a given node to any of its descendants contains
 *    the same # of black nodes.
 */
uint8_t redblack_tree_valid(RedblackTree *tree) {
    /* NOTE: we assume 1 is always true */
    if (tree->root->color != REDBLACK_BLACK) {
        return FALSE;
    }

    return redblack_validate_node(tree, tree->root);
}


START_TEST (test_redblack_create) {
    RedblackTree *tree = redblack_create(integer_comparator);
    fail_unless(redblack_tree_valid(tree));
}
END_TEST

Suite*
redblack_suite(void) {
    Suite *s = suite_create("Redblack Tree");

    /* Core test case */
    TCase *tc_core = tcase_create("Redblack Tree");
    tcase_add_test(tc_core, test_redblack_create);

    suite_add_tcase(s, tc_core);
    return s;
}

