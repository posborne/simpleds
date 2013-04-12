/*
 * deque.c
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
 * An implementation of a deque (double ended queue) in pure C.
 */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "deque.h"
#ifndef DEQUE_STATIC
#include <malloc.h>
#endif /* DEQUE_STATIC */

/* The default comparator which simplies does a simple comparison based on
 * memory address.  It is really only useful to compare if two pointers point
 * to the same piece of data, beyond that less than or equal are not very
 * useful.
 */
static int8_t
default_comparator(const void * a, const void * b) {
	if (a == b) {
		return 0;
	} else {
		return a > b ? 1 : -1;
	}
}

/* Initialize a deque that has been already allocated
 *
 * This is called automatically if deque_create() is being used to
 * allocate the deque.
 */
void
deque_init(Deque d, deque_comparater_t compare_func) {
	assert(d != NULL);

	/* store a pointer to the comparison function */
	if (compare_func == NULL) {
		d->compare_func = default_comparator;
	} else {
		d->compare_func = compare_func;
	}
	
	/* initialize the state of the rest of the deque */
	d->head = NULL;
	d->tail = NULL;
	d->number_items = 0;
}


#ifdef DEQUE_STATIC
static struct deque_node_t *
deque_alloc_node(Deque d) {
	struct deque_node_t * node;
	int i;
	/* find the first unused node */		
	while ((node = &d->nodes[i++])->in_use);
	if (node != NULL)
		node->in_use = true;
	return node;
}

static void
deque_free_node(struct deque_node_t * node) {
	node->in_use = false;
}
#else
#include <malloc.h>
static struct deque_node_t *
deque_alloc_node(Deque d) {
	return (struct deque_node_t *)malloc(sizeof(struct deque_node_t));
}

static void
deque_free_node(struct deque_node_t * node)
{
	free(node);
}

/* Create a deque and return a reference, if memory cannot be allocated for
 * the deque, NULL will be returned.
 * 
 * A comparison function is passed in.  Given two pointers a and b to the
 * value of nodes in the deque the comparison function should meet the following
 * criterion:
 * 
 * x = f(a,b) -> {x > 0 if a > b,
 * 				  x < 0 if a < b,
 * 	              x = 0 if a = b}
 * 
 * If the compare_func is NULL a default comparison function which only
 * compares the memory address of the items will be used.
 */
Deque
deque_create(deque_comparater_t compare_func) {
	Deque d = malloc(sizeof(struct deque_t));
	if (d != NULL) {
		deque_init(d, compare_func);
	}
	return d;
}


/* Copy the deque and return a reference to the new deque
 *
 * This is a shallow copy, so only the deque container data structures are
 * copied, not the values referenced.
 */
Deque
deque_copy(Deque d) {
    Deque newDeque;
    DequeNode tmp;
    newDeque = deque_create(d->compare_func);
    tmp = d->head;
    while (tmp != NULL) {
        deque_append(newDeque, tmp->value);
        tmp = tmp->next;
    }
    return newDeque;
}
#endif /* DEQUE_STATIC */

/* Free the data allocated for the deque and all nodes */
void
deque_free(Deque d) {
	deque_clear(d);
	free(d);
}

/* Append the specified item to the right end of the deque (head).
 */
deque_result_t
deque_append(Deque d, void* item) {
	deque_result_t retcode;	
	DequeNode newNode;
	assert(d != NULL);

	/* allocate memory for the new node and put it in a valid state */
	newNode = deque_alloc_node(d);
	if (newNode == NULL) {
		retcode = DEQUE_ALLOC_ERROR;
	} else {
		newNode->prev = d->head;
		newNode->next = NULL;
		newNode->value = item;
	
		if (d->head != NULL) {
			d->head->next = newNode;
		}
		if (d->tail == NULL) {
			d->tail = newNode; /* only one item */
		}
		d->head = newNode;
		d->number_items++;
	}
	return retcode;
}

/* Append the specified item to the left end of the deque (tail). 
 */
deque_result_t
deque_appendleft(Deque d, void* item) {
	DequeNode newNode;
	deque_result_t retcode = DEQUE_SUCCESS;
	assert(d != NULL);

	/* create the new node and put it in a valid state */
	newNode = deque_alloc_node(d);
	if (newNode == NULL) {
		retcode = DEQUE_ALLOC_ERROR;
	} else {
		newNode->next = d->tail;
		newNode->prev = NULL;
		newNode->value = item;
	
		if (d->tail != NULL) {
			d->tail->prev = newNode;
		}
		if (d->head == NULL) {
			d->head = d->tail;
		}
		d->tail = newNode;
		d->number_items++;
	}
	return retcode;
}

/* Clear the specified deque, this will free all data structures related to the
 * deque itself but will not free anything free the items being pointed to.
 * 
 * This operation is O(n) where n is the number of elements in the deque.
 */
deque_result_t
deque_clear(Deque d) {
	DequeNode tmp;
	assert(d != NULL);
	while (d->head != NULL) {
		tmp = d->head;
		d->head = tmp->next;
		deque_free_node(tmp);
	}
	d->head = NULL;
	d->tail = NULL;
	d->number_items = 0;
	return DEQUE_SUCCESS;
}

/* Remove the rightmost element from the deque and return a reference to the
 * value pointed to by the deque node.  If there is no rightmost element
 * then NULL will be returned.
 * 
 * This operation is O(1), constant time.
 */
void*
deque_pop(Deque d) {
	DequeNode prevHead;
	void* value;
	if ((prevHead = d->head) == NULL) {
		return NULL;
	} else {
		d->head = prevHead->prev;
		d->number_items--;
		value = prevHead->value;
		deque_free_node(prevHead);
		return value;
	}
}

/* Get the value of the deque tail or NULL if the deque is empty */
void*
deque_peek(Deque d) {
	if (d->head == NULL) {
		return NULL;
	} else {
		return d->head->value;
	}
}

/* Remove the leftmost element from the deque and return a reference to the
 * value pointed to by the deque node.  If there is no leftmost element
 * then NULL will be returned.
 * 
 * This operation is O(1), constant time.
 */
void*
deque_popleft(Deque d) {
	DequeNode prevTail;
	void* value;
	if (d->tail == NULL) {
		return NULL;
	} else {
		prevTail = d->tail;
		d->tail = prevTail->next;
		if (d->tail != NULL) {
			d->tail->prev = NULL;
		}
		d->number_items--;
		value = prevTail->value;
		deque_free_node(prevTail);
		return value;
	}
}

/* Get the value of the deque head (leftmost element) or NULL if empty */
void*
deque_peekleft(Deque d) {
	if (d->tail == NULL) {
		return NULL;
	} else {
		return d->tail->value;
	}
}

/* Remove the first occurrence of item from the deque, starting from the left.
 * A reference to the removed node value will be returned, otherwise NULL
 * will be returned (if the item cannot be found).
 * 
 * Note that the comparison is done on the values of the data pointers, so
 * even if I had two strings "foo" and "foo" at different places in memory,
 * we would not get a match.
 * 
 * This operation executes in O(n) time where n is the number of elements in
 * the deque due to a linear search for the item.
 */
void*
deque_remove(Deque d, void* item) {
	void* value;
	DequeNode tmp = d->tail;
	while (tmp != NULL) {
		if ((d->compare_func)(tmp->value, item) == 0) {
			value = tmp->value;
			if (tmp->prev != NULL) {
				tmp->prev->next = tmp->next;
			}
			if (tmp->next != NULL) {
				tmp->next->prev = tmp->prev;
			}
			deque_free_node(tmp);
			d->number_items--;
			return value;
		}
		tmp = tmp->next;
	}
	return NULL; /* item not found in deque */
}

/* Rotate the deque n steps to the right.  If n is negative, rotate the deque
 * to the left.  Here is a set of equivalent operations that gives you an idea
 * of what the rotate operations:
 * 
 * -- These are equivalent --
 * deque_rotate(d, 1);
 * deque_rotateright(d, 1);
 * deque_appendleft(deque_pop());
 * 
 * -- These are equivalent --
 * deque_rotate(d, -1);
 * deque_rotateleft(d, 1);
 * deque_append(deque_popleft());
 * 
 * The rotate operation is O(m) where m is the absolute value of the number of
 * steps we are rotating.
 */
void
deque_rotate(Deque d, int32_t n) {
	if (n > 0) {
		deque_rotateright(d, n);
	} else if (n < 0) {
		deque_rotateleft(d, abs(n));
	}
}

/* Rotate the deque n steps to the right */
void
deque_rotateright(Deque d, uint32_t n) {
	int i;
	DequeNode tmp;
	
	/* make sure we don't segfault */
	if (d->head == NULL || d->tail == NULL) {
		return;
	}
	
	/* do the rotations */
	for (i = 0; i < n; i++) {
		tmp = d->head;
		d->head = tmp->prev;
		d->head->next = NULL;
		
		tmp->next = d->tail;
		tmp->prev = NULL;
		d->tail->prev = tmp;
		d->tail = tmp;
	}
}

/* Rotate the deque n steps to the left */
void
deque_rotateleft(Deque d, uint32_t n) {
	int i;
	DequeNode tmp;
	
	/* make sure we don't segfault */
	if (d->head == NULL || d->tail == NULL) {
		return;
	}
	
	/* do the rotations */
	for (i = 0; i < n; i++) {
		tmp = d->tail;
		d->tail = tmp->next;
		d->tail->prev = NULL;
		
		tmp->prev = d->head;
		tmp->next = NULL;
		d->head->next = tmp;
		d->head = tmp;
	}
}

/* Return the number of items in the deque */
uint32_t
deque_count(Deque d) {
	return d->number_items;
}

/* Reverse the order of the items in the deque */
void
deque_reverse(Deque d) {
	DequeNode currNode;
	DequeNode nextNode;
	currNode = d->tail;
	while (currNode != NULL) {
	    nextNode = currNode->next;
		currNode->next = currNode->prev;
		currNode->prev = nextNode;
		currNode = nextNode;
	}
	
	/* flip head and tail */
	currNode = d->tail;
	d->tail = d->head;
	d->head = currNode;
}

/* Return TRUE if the deque contains the specified item and FALSE if not */
uint8_t
deque_contains(Deque d, void* item) {
	DequeNode tmp = d->tail;
	while (tmp != NULL) {
		if ((d->compare_func)(tmp->value, item) == 0) {
			return TRUE;
		}
		tmp = tmp->next;
	}
	return FALSE; /* item not found in deque */
}

