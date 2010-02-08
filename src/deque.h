/* 
 * deque.h
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

#ifndef DEQUE_H
#define DEQUE_H

#include <stdint.h>

#define DEQUE_SUCCESS 0
#define DEQUE_FAILURE 1

#define TRUE  1
#define FALSE 0

struct deque_node_t {
	void* value;
	struct deque_node_t *next;
	struct deque_node_t *prev;
};

struct deque_t {
	struct deque_node_t *head;
	struct deque_node_t *tail;
	uint32_t number_items;
	int8_t(*compare_func)(const void *, const void *);
};

typedef struct deque_node_t *DequeNode;
typedef struct deque_t *Deque;

Deque    deque_create(int8_t(*compare_func)(const void*, const void*));
void     deque_free(Deque d);
uint8_t  deque_append(Deque d, void* item);
uint8_t  deque_appendleft(Deque d, void* item);
uint8_t  deque_clear(Deque d);
void*    deque_peek(Deque d);
void*    deque_pop(Deque d);
void*    deque_peekleft(Deque d);
void*    deque_popleft(Deque d);
void*    deque_remove(Deque d, void* item);
void     deque_rotate(Deque d, int32_t n);
void     deque_rotateleft(Deque d, uint32_t n);
void     deque_rotateright(Deque d, uint32_t n);
uint32_t deque_count(Deque d);
Deque    deque_copy(Deque d);
void     deque_reverse(Deque d);
uint8_t  deque_contains(Deque d, void* item);

#endif
