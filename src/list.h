#ifndef LIST_H
#define LIST_H
#include <stdint.h>

#define LIST_TYPE_FIXED 0x00
#define LIST_TYPE_EXPANDING 0x01
#define LIST_SUCCESS 0x00
#define LIST_ERROR 0x01
#define LIST_INDEX_ERROR 0x02

typedef struct _list_t {
	void **ptr_table;
	uint32_t number_items;
	uint32_t capacity;
	uint8_t list_type;
} ListType;
typedef ListType *ArrayList;

ArrayList list_create();
ArrayList list_create_heap();
ArrayList list_create_heap_size(const uint32_t items);
ArrayList list_create_static(const void *dataPtr, const uint32_t size);
uint8_t list_free(ArrayList list);
void* list_getitem(ArrayList list, const int index);
uint8_t list_append(ArrayList list, void *item);
uint8_t list_extend(ArrayList list, const ArrayList appendList);
uint8_t list_insert(ArrayList list, const int index, void *item);
void* list_remove(ArrayList list, const void *item);
void* list_pop(ArrayList list);
void* list_pop_item(ArrayList list, const int index);
int list_index(ArrayList list, const void* item);
uint32_t list_count(ArrayList list);
void list_reverse(ListType *listPtr);
void list_quicksort(ArrayList list, int8_t(*compare_func)(void*, void*), 
				   uint32_t left, uint32_t right);
void list_sort(ArrayList list, int8_t(*compare_func)(void*, void*));

#endif
