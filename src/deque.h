#ifndef DEQUE_H
#define DEQUE_H

#include <stdint.h>

struct deque_node_t {
	void* value;
	struct deque_node_t *next;
	struct deque_node_t *prev;
};

struct deque_t {
	struct deque_node_t *head;
	struct deque_node_t *tail;
	uint32_t number_items;
};

typedef struct deque_node_t *DequeNode;
typedef struct deque_t *Deque;

Deque   deque_create();
uint8_t deque_append(Deque d, void* item);
uint8_t deque_appendleft(Deque d, void* item);
uint8_t deque_clear(Deque d);
void*   deque_pop(Deque d);
void*   deque_popleft(Deque d);
void*   deque_remove(Deque d, void* item);
void*   deque_rotate(Deque d, int32_t n);
void*   deque_count(Deque d);
Deque   deque_copy(Deque d);
void    deque_reverse(Deque d);
uint8_t deque_contains(Deque d);

#endif
