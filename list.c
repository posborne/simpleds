#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "list.h"

#define DEFAULT_LIST_SIZE 10

/* Module Local Function Prototypes */
static uint8_t list_memcheck(ListType *listPtr);

ArrayList
list_create() {
    return list_create_heap();
}

ArrayList
list_create_heap() {
    return list_create_heap_size(DEFAULT_LIST_SIZE);
}

ArrayList
list_create_heap_size(const uint32_t items) {
    /* allocate memory for items buckets */
    ArrayList list = malloc(sizeof(ListType));
    void *ptr_table = malloc(items * sizeof(void*));
    list->ptr_table = ptr_table;
    list->number_items = 0;
    list->capacity = items;
    return list;
}

ArrayList
list_create_static(const void *dataPtr) {
    ArrayList list = malloc(sizeof(ListType));
    list->ptr_table = (void*)dataPtr;
    list->number_items = 0;
    return list;
}

uint8_t
list_free(ArrayList list) { 
    free(list->ptr_table);
    free(list);
    return LIST_SUCCESS;
}

uint8_t
list_append(ArrayList list, void *item) {
    /* make sure we have the room to expand */
    uint8_t result_code;
    if ((result_code = list_memcheck(list)) != LIST_SUCCESS) {
	return result_code;
    }
    
    /* all clear at this point, append away */
    list->ptr_table[list->number_items] = item;
    list->number_items++;
    return LIST_SUCCESS;
}

/* Extend a list by appending all the elements from a second list */
uint8_t 
list_extend(ArrayList list, const ArrayList appendList) {
    /* Do we have enough room to go through with this for fixed size? */
    if (list->list_type == LIST_TYPE_FIXED &&
	list->capacity < (list->number_items + appendList->number_items)) {
	return LIST_ERROR;
    }
    
    /* append each of the items from the second list */
    int i;
    for (i = 0; i < appendList->number_items; i++) {
	list_append(list, list_getitem(appendList, i));
    }
    return LIST_SUCCESS;
}

void*
list_getitem(ArrayList list, const int index) {
    if (index > list->number_items) {
	return NULL;
    } else {
	return list->ptr_table[index];
    }
}

uint8_t
list_insert(ArrayList list, const int index, void *item) {
    uint8_t result_code;
    
    /* index in valid range? */
    if (index < 0 || index > list->number_items) {
	return LIST_INDEX_ERROR;
    }
    
    /* expand ptr_table if needed */
    if ((result_code = list_memcheck(list)) != LIST_SUCCESS) {
	return result_code;
    }

    /* shift things around in the table for the newcomer */
    memcpy(list->ptr_table + index * sizeof(void*),
	   list->ptr_table + (index + 1) * sizeof(void*),
	   (list->number_items - index) * sizeof(void*));
    
    list->ptr_table[index] = item;
    
    return LIST_SUCCESS;
}

/* Remove the first instance of item from the list */
void*
list_remove(ArrayList list, const void* item) {
    int i;
    void **tmp;
    for (i = 0; i < list->number_items; i++) {
	tmp = list->ptr_table + sizeof(void*) * i;
	if (tmp == item) {
	    return list_pop_item(list, i);
	}
    }
    return NULL;
}

/* Pop the rightmost element from the list */
void*
list_pop(ArrayList list) {
    return list_pop_item(list, list->number_items - 1);
}

/* Pop the item at the specified index and assign it to item */
void*
list_pop_item(ArrayList list, const int index) {
    void* popped_item = NULL;
    if (index < 0 || index >= list->number_items) {
	return NULL;
    }
    
    /* copy the ptr while we still have access then overwrite */
    popped_item = list->ptr_table + sizeof(void*) * index;
    memcpy(list->ptr_table + (index + 1) * sizeof(void*),
	   list->ptr_table + index * sizeof(void*),
	   (list->number_items - index) * sizeof(void*));
    return popped_item;
}

/* Assign to index the index of the first instance of item */
int32_t
list_index(ArrayList list, const void *item) {
    int i;
    void *tmp;
    for (i = 0; i < list->number_items; i++) {
	tmp = list->ptr_table + sizeof(void*) * i;
	if (tmp == item) {
	    return (int32_t) i;
        }
    }
    return -1; /* Error */
}

/* Return the number of items in the list */
uint32_t
list_count(ArrayList list) {
    return list->number_items;
}

/* Reverse the list in place */
void
list_reverse(ArrayList list) {
    int i;
    void *tmp, **beginPtr, **endPtr;
    uint32_t last_index = list->number_items - 1;
    for (i = 0; i < list->number_items / 2; i++) {
	tmp = list->ptr_table + sizeof(void*) * i;
	beginPtr = list->ptr_table + sizeof(void*) * i;
	endPtr = list->ptr_table + sizeof(void*) * (last_index - i);
	*endPtr = *beginPtr;
	*beginPtr = tmp;
    }
}

void
list_sort(ArrayList list, int8_t (*sort_func)(void*, void*)) {
    // TODO: what sorting algorithm should we use?
}

/**** Module Local Function Implementations ****/

/* Check to see if we need to expand the ptr_table */
static uint8_t
list_memcheck(ArrayList list) {
    if (list->capacity - list->number_items <= 0) {
	if (list->list_type == LIST_TYPE_FIXED) {
	    return LIST_ERROR;
        } else {
	    /* double the amount of memory allocated */
	    void *new_table = malloc(sizeof(void*) * 2 * list->capacity);
	    memcpy(new_table, list->ptr_table, sizeof(void*) * list->capacity);
	    free(list->ptr_table);
	    list->ptr_table = new_table;
	    list->capacity *= 2;
	}
    }
    assert(list->capacity - list->number_items > 0);
    return LIST_SUCCESS;
}

/**************************************************************/
/* TESTS */
int main(void) {
    printf("Executing a simple list test\n");
    ArrayList list = list_create();
    char* item1 = "Hello";
    char* item2 = "WORLD";
    list_append(list, item1);
    list_append(list, item2);
    
    printf("List has %d items\n", list_count(list));
    printf("First item: %s\n", (char*)list->ptr_table[0]);

    int i;
    for (i = 0; i < list_count(list); i++) {
	printf("Got item %s at index %d\n", (char*)list_getitem(list, i), i);
    }    
    list_free(list);

    return 0;
}
