/* 
 * redblack.h
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

#ifndef REDBLACK_HEADER_H
#define REDBLACK_HEADER_H

#include <stdint.h>

enum redblack_iteration_order {
    REDBLACK_ITERATE_PREORDER,
    REDBLACK_ITERATE_INORDER,
    REDBLACK_ITERATE_POSTORDER
};

#define REDBLACK_RED   0
#define REDBLACK_BLACK 1

typedef struct redblack_node {
    uint8_t color; /* 0 if black, 1 if red */
    struct redblack_node *left;
    struct redblack_node *right;
    void *value;
} RedblackNode;

typedef struct redblack_iterator {
    enum redblack_iteration_order iteration_order;
    RedblackNode* currentNode;
} RedblackIterator;

typedef struct redblack_tree {
    RedblackNode* root;
    RedblackNode* nil; /* nil node for this tree */
    RedblackNode *current;
    RedblackNode *parent;
    RedblackNode *grandparent;
    RedblackNode *great_grandparent;
    int8_t(*compare_func)(const void*, const void*);
} RedblackTree;

/* function prototypes */
RedblackTree* redblack_create(int8_t(*compare_func)(const void*, const void*));
void redblack_insert(RedblackTree *tree, void* item);
void* redblack_remove(RedblackTree *tree, void* item);
int8_t redblack_contains(RedblackTree *tree, void* item);

/* iterator functions */
RedblackIterator* redblack_iterator_create(RedblackTree* tree, uint8_t iteration_order);
void redblack_iterator_free(RedblackIterator* iterator);
void redblack_iterator_next(RedblackIterator* iterator);
int8_t redblack_iterator_hasnext(RedblackIterator* iterator);

#endif
