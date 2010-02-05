/*
 * deque.c
 * 
 * An implementation of a deque (double ended queue) in pure C.
 * 
 * Created On: Jan 18, 2009
 *     Author: Paul Osborne <osbpau@gmail.com>
 */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "deque.h"


/* Create a deque and return a reference, if memory cannot be allocated for
 * the deque, NULL will be returned.
 */
Deque
deque_create() {
	Deque d = malloc(sizeof(struct deque_node_t));
	if (d == NULL) {
		return NULL;
	} else {
		d->head = NULL;
		d->tail = NULL;
		d->number_items = 0;
		return d;
	}
}

/* Free the data allocated for the deque and all nodes */
void
deque_free(Deque d) {
	deque_clear(d);
	free(d);
}

/* Append the specified item to the right end of the deque (head).
 */
uint8_t
deque_append(Deque d, void* item) {
	DequeNode newNode;
	assert(d != NULL);
	newNode = malloc(sizeof(struct deque_node_t));
	newNode->prev = d->head;
	newNode->next = NULL;
	newNode->value = item;
	
	d->head = newNode;
	d->number_items++;
	return DEQUE_SUCCESS;
}

/* Append the specified item to the left end of the deque (tail). 
 */
uint8_t
deque_appendleft(Deque d, void* item) {
	DequeNode newNode;
	assert(d != NULL);
	newNode = malloc(sizeof(struct deque_node_t));
	newNode->next = d->tail;
	newNode->prev = NULL;
	newNode->value = item;
	
	d->tail = newNode;
	if (d->head == NULL) {
		d->head = d->tail;
	}
	d->number_items++;
	return DEQUE_SUCCESS;
}

/* Clear the specified deque, this will free all data structures related to the
 * deque itself but will not free anything free the items being pointed to.
 * 
 * This operation is O(n) where n is the number of elements in the deque.
 */
uint8_t
deque_clear(Deque d) {
	DequeNode tmp;
	assert(d != NULL);
	while ((tmp = d->head) != NULL) {
		d->head = tmp->next;
		free(tmp);
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
		free(prevHead);
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
	if ((prevTail = d->tail) == NULL) {
		return NULL;
	} else {
		d->tail = prevTail->next;
		d->number_items--;
		value = prevTail->value;
		free(prevTail);
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
	DequeNode tmp = d->head;
	while (tmp != NULL) {
		if (tmp == item) {
			value = tmp->value;
			if (tmp->prev != NULL) {
				tmp->prev = tmp->next;
			}
			if (tmp->next != NULL) {
				tmp->next->prev = tmp->prev;
			}
			free(tmp);
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
		deque_rotateleft(d, n);
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
		d->head = tmp;
	}
}

/* Return the number of items in the deque */
uint32_t
deque_count(Deque d) {
	return d->number_items;
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
	newDeque = deque_create();
	tmp = d->head;
	while (tmp != NULL) {
		deque_append(newDeque, tmp->value);
		tmp = tmp->next;
	}
	return newDeque;
}

/* Reverse the order of the items in the deque */
void
deque_reverse(Deque d) {
	DequeNode currNode;
	DequeNode nextNode;
	currNode = d->head;
	while (currNode == NULL) {
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
	DequeNode tmp = d->head;
	while (tmp != NULL) {
		if (tmp == item) {
			return TRUE;
		}
		tmp = tmp->next;
	}
	return FALSE; /* item not found in deque */
}
