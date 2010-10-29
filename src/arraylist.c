/* 
 * arraylist.c
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
 * An implementation of an indexed list.  The interface for this list is
 * modeled after the python standard library list as well as the Java
 * ArrayList class.
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "arraylist.h"

#define DEFAULT_ARRAYLIST_SIZE 10

/*
 *  Module Local Function Prototypes and MACROS
 */
inline void PTR_SWAP(void **a, void **b) {
    void *t = *a;
    *a = *b;
    *b = t;
}

/* Check to see if we need to expand the ptr_table */
static uint8_t arraylist_memcheck(ArrayList list) {
    if (list->capacity - list->number_items <= 0) {
        if (list->list_type == ARRAYLIST_TYPE_FIXED) {
            return ARRAYLIST_ERROR;
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
    return ARRAYLIST_SUCCESS;
}

/*
 * Interface function implementations
 */
ArrayList arraylist_create(int8_t(*compare_func)(void*, void*)) {
    return arraylist_create_heap(compare_func);
}

/* Create an arraylist with memory allocated on the heap.
 * 
 * This function calls arraylist_create_heap_size() under the hood with a starting
 * list size of 10.  If the list size is known to be much different than this
 * it is recommended to called arraylist_create_heap_size() directly as this will
 * reduce the number of memory reallocations that need to be performed.
 */
ArrayList arraylist_create_heap(int8_t(*compare_func)(void*, void*)) {
    return arraylist_create_heap_size(DEFAULT_ARRAYLIST_SIZE, compare_func);
}

/* Create a list with memory allocated on the heap
 * 
 * The number of items specified determines the initial maximum capacity for
 * the list (this is different from the item count which always starts at 0).
 * This list will automatically grow as needed, but specifying an initial size
 * large enough for all data will reduce the number of expensive memory
 * reallocations that are needed.
 * 
 * When done with a list, arraylist_free() should be called in order to prevent
 * a memory leak.  This will free that data structures used to track the list
 * items, but it will not free the elements referenced by the list.
 */
ArrayList arraylist_create_heap_size(const uint32_t items,
        int8_t(*compare_func)(void*, void*)) {
    /* allocate memory for items buckets */
    ArrayList list = malloc(sizeof(ListType));
    void *ptr_table = malloc(items * sizeof(void*));
    list->ptr_table = ptr_table;
    list->number_items = 0;
    list->capacity = items;
    list->list_type = ARRAYLIST_TYPE_EXPANDING;
    list->compare_func = compare_func;
    return list;
}

/* Create a list with ptr_table memory allocated statically
 * 
 * In embedded systems, it is often desirable to be able to statically allocate
 * memory up-front.  Here we allow the user to pass in a buffer that should be
 * used for storing data pointers.
 * 
 * Unlike lists where memory is allocated on the heap, lists allocated
 * statically will not automatically expand when they fill up.  It is up to the
 * user to either ensure that the number of items added to the list will not
 * exceed the size of the buffer allocated or to catch the error when it happens
 * and copy the data to a new list with more memory allocated.
 */
ArrayList arraylist_create_static(const void *dataPtr, const uint32_t size,
        int8_t(*compare_func)(void*, void*)) {
    ArrayList list = malloc(sizeof(ListType));
    list->ptr_table = (void*) dataPtr;
    list->number_items = 0;
    list->capacity = size;
    list->list_type = ARRAYLIST_TYPE_FIXED;
    list->compare_func = compare_func;
    return list;
}

/* Free the list and the list's pointer buffer
 * 
 * arraylist_free() will not free objects referenced in the list, if the list is the
 * only reference to these objects, the user should walk through the list and
 * free all of these objects first.
 */
uint8_t arraylist_free(ArrayList list) {
    free(list->ptr_table);
    free(list);
    return ARRAYLIST_SUCCESS;
}

/* Append the specified item to the list
 * 
 * If this list was allocated on the heap, the list will automatically expand
 * as needed to fit the new data.  If not, the function call will return an
 * error code, most likely ARRAYLIST_ERROR.  ARRAYLIST_SUCCESS indicates that the append
 * completed successfully.
 */
uint8_t arraylist_append(ArrayList list, void *item) {
    /* make sure we have the room to expand */
    uint8_t result_code;
    if ((result_code = arraylist_memcheck(list)) != ARRAYLIST_SUCCESS) {
        return result_code;
    }

    /* all clear at this point, append away */
    list->ptr_table[list->number_items] = item;
    list->number_items++;
    return ARRAYLIST_SUCCESS;
}

/* Extend a list by appending all the elements from a second list
 * 
 * As with append(), if the list is fixed size, then an error code will be
 * returned if the allocated space runs out.  Additional space will be allocated
 * for lists whose buffer was allocated on the heap.
 */
uint8_t arraylist_extend(ArrayList list, const ArrayList appendList) {
    int i;
    /* Do we have enough room to go through with this for fixed size? */
    if (list->list_type == ARRAYLIST_TYPE_FIXED && list->capacity
            < (list->number_items + appendList->number_items)) {
        return ARRAYLIST_ERROR;
    }

    /* append each of the items from the second list */
    for (i = 0; i < appendList->number_items; i++) {
        arraylist_append(list, arraylist_getitem(appendList, i));
    }
    return ARRAYLIST_SUCCESS;
}

/* Get the item at the specified index
 * 
 * If the index is out of bounds, return NULL.
 */
void*
arraylist_getitem(ArrayList list, const int index) {
    if (index > list->number_items) {
        return NULL;
    } else {
        return list->ptr_table[index];
    }
}

/* Insert an element into the list at the specified index
 * 
 * The list will be expanded as needed (or an error thrown if the list is
 * fixed size).  All elements right of the index specified will be shifted
 * right by one index in order to make space.
 * 
 * Note that inserting a value at index 0 is an order-N operation (meaning
 * that we must make N copies for each of the items in the list).  If this is
 * an operation that needs to be performed often, consider uisng a different
 * data structure such as a stack, queue, or deque.
 */
uint8_t arraylist_insert(ArrayList list, const int insert_index, void *item) {
    int i;
    uint8_t result_code;

    /* i in valid range? */
    if (insert_index < 0 || insert_index > list->number_items) {
        return ARRAYLIST_INDEX_ERROR;
    }

    /* expand ptr_table if needed */
    if ((result_code = arraylist_memcheck(list)) != ARRAYLIST_SUCCESS) {
        return result_code;
    }

    /* shift things around in the table for the newcomer */
    for (i = list->number_items; i > insert_index; i--) {
        list->ptr_table[i] = list->ptr_table[i - 1];
    }

    list->ptr_table[insert_index] = item;
    list->number_items++;

    return ARRAYLIST_SUCCESS;
}

/* Remove the first instance of item from the list
 * 
 * We return a reference to the item that has been removed from the list or
 * NULL if the item could not be found.  When the item is removed, elements
 * right of the one removed from the list will be shifted left and the size
 * of the list updated to maintain list invariants.
 */
void*
arraylist_remove(ArrayList list, const void* item) {
    int i;
    void **tmp;
    for (i = 0; i < list->number_items; i++) {
        tmp = list->ptr_table[i];
        if (tmp == item) {
            return arraylist_pop_item(list, i);
        }
    }
    return NULL;
}

/* Pop the rightmost element from the list
 * 
 * Remove the rightmost item from the list and return a reference to it.  If
 * the list has no elements, NULL will be returned.
 */
void*
arraylist_pop(ArrayList list) {
    return arraylist_pop_item(list, list->number_items - 1);
}

/* Pop the item at the specified index and assign it to item */
void*
arraylist_pop_item(ArrayList list, const int popIndex) {
    int i;
    void* popped_item = NULL;
    if (popIndex < 0 || popIndex >= list->number_items) {
        return NULL;
    }

    /* copy the ptr while we still have access then overwrite */
    popped_item = list->ptr_table[popIndex];

    /* shift items to the right left by one */
    for (i = popIndex; i < list->number_items; i++) {
        list->ptr_table[i] = list->ptr_table[i + 1];
    }
    list->number_items--;

    return popped_item;
}

/* Get the index of the first item equal to the specified item
 * 
 * If an item cannot be found in the list that does not match the specified
 * index, then -1 is returned.
 */
int32_t arraylist_index(ArrayList list, const void *item) {
    int i;
    for (i = 0; i < list->number_items; i++) {
        if (list->ptr_table[i] == item) {
            return (int32_t) i;
        }
    }
    return -1; /* Error */
}

/* Return the number of items in the list */
uint32_t arraylist_count(ArrayList list) {
    return list->number_items;
}

/* Reverse the list in place */
void arraylist_reverse(ArrayList list) {
    int i;
    int lastIndex = list->number_items - 1;
    for (i = 0; i < (lastIndex + 1) / 2; i++) {
        PTR_SWAP(&list->ptr_table[i], &list->ptr_table[lastIndex - i]);
    }
}

/* Determine whether the arraylist contains the specified item 
 * 
 * Return TRUE (1), if the value is in arraylist and FALSE (0) if not.
 */
uint8_t arraylist_contains(ArrayList list, void *item) {
    return arraylist_index(list, item) >= 0;
}

/* Perform the quicksort partition step for in-place quicksort
 * 
 * Here we partition a part of the list around the specified pivotIndex.
 * After execution of this function it is true that the values contained in
 * list left of the pivot will be less than the pivot value and the values
 * in the list right of the pivot will be greater than or equal to the
 * pivot value.  The function returns the final storeIndex.
 */
static uint32_t quicksort_partition(ArrayList list, uint32_t left,
        uint32_t right) {
    uint32_t i, j, pivotIndex;
    void* pivot;
    j = left;

    assert(right > left);
    pivotIndex = (left + right) / 2;
    pivot = list->ptr_table[pivotIndex]; /* use middle as pivot */
    PTR_SWAP(&list->ptr_table[pivotIndex], &list->ptr_table[right]);
    for (i = left; i < right; i++) {
        if ((*list->compare_func)(list->ptr_table[i], pivot) < 0) {
            PTR_SWAP(&list->ptr_table[i], &list->ptr_table[j++]);
        }
    }
    PTR_SWAP(&list->ptr_table[right], &list->ptr_table[j]);
    return j;
}

/* Perform a quicksort on the provided list (beginning at left, ending at right)
 * 
 * After execution the items in the provided list will be in order from least
 * to greatest according to the order defined in the compare_func function
 * pointer passed in.
 * 
 * This implementation of quicksort is done in-place.  This means that the sort
 * is not stable, but in actuality yields much better performance than an
 * implementation which is not done in-place.  Also, this implementation is
 * O(1) with regards to space.
 */
void arraylist_quicksort(ArrayList list, uint32_t leftIndex,
        uint32_t rightIndex) {
    if (leftIndex < rightIndex) {
        uint32_t pivotOn = quicksort_partition(list, leftIndex, rightIndex);
        arraylist_quicksort(list, leftIndex, pivotOn); /* left side */
        arraylist_quicksort(list, pivotOn + 1, rightIndex); /* right side */
    }
}

/* Perform a sort on the list using the provided compare_func
 * 
 * The default sort used is an in-place quicksort which does very well in most
 * use cases.
 */
void arraylist_sort(ArrayList list) {
    arraylist_quicksort(list, 0, list->number_items - 1);
}
