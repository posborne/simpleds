#include <stdint.h>

typedef struct _list_t {
    void     *ptr_table;
    uint32_t number_items;
} ListType;

uint8_t list_create_heap(ListType *listPtr);
uint8_t list_create_heap_size(ListType *listPtr, uint8_t items);
uint8_t list_create_static(ListType *listPtr, void *dataPtr);
uint8_t list_free(ListType* listPtr);
uint8_t list_append(ListType *listPtr, void *item);
uint8_t list_extend(ListType *listPtr, ListType *appendList);
uint8_t list_insert(ListType *listPtr, int index, void *item);
uint8_t list_remove(ListType *listPtr, void* item);
uint8_t list_pop(ListType *listPtr);
uint8_t list_pop_item(ListType *listPtr, int index);
uint8_t list_index(ListType *listPtr, void* item);
uint32_t list_count(ListType *listPtr);
uint8_t list_reverse(ListType *listPtr);
uint8_t list_sort(ListType *listPtr, int8_t (*sort_func)(void*, void*));

