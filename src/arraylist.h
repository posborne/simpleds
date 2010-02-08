/* 
 * arraylist.h
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

#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stdint.h>

#define ARRAYLIST_TYPE_FIXED 0x00
#define ARRAYLIST_TYPE_EXPANDING 0x01
#define ARRAYLIST_SUCCESS 0x00
#define ARRAYLIST_ERROR 0x01
#define ARRAYLIST_INDEX_ERROR 0x02

typedef struct _list_t {
	void **ptr_table;
	uint32_t number_items;
	uint32_t capacity;
	uint8_t list_type;
	int8_t(*compare_func)(void*, void*);
} ListType;
typedef ListType *ArrayList;

ArrayList arraylist_create(int8_t(*compare_func)(void*, void*));
ArrayList arraylist_create_heap(int8_t(*compare_func)(void*, void*));
ArrayList arraylist_create_heap_size(const uint32_t items, 
								     int8_t(*compare_func)(void*, void*));
ArrayList arraylist_create_static(const void *dataPtr, const uint32_t size,
								  int8_t(*compare_func)(void*, void*));
uint8_t arraylist_free(ArrayList list);
void* arraylist_getitem(ArrayList list, const int index);
uint8_t arraylist_append(ArrayList list, void *item);
uint8_t arraylist_extend(ArrayList list, const ArrayList appendList);
uint8_t arraylist_insert(ArrayList list, const int index, void *item);
void* arraylist_remove(ArrayList list, const void *item);
void* arraylist_pop(ArrayList list);
void* arraylist_pop_item(ArrayList list, const int index);
int arraylist_index(ArrayList list, const void* item);
uint8_t arraylist_contains(ArrayList list, void *item);
uint32_t arraylist_count(ArrayList list);
void arraylist_reverse(ListType *listPtr);
void arraylist_quicksort(ArrayList list, uint32_t left, uint32_t right);
void arraylist_sort(ArrayList list);

#endif
