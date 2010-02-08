/*
 * test_arraylist.c
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

#include <check.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "tests.h"
#include "../src/arraylist.h"

/** TEST UTILITY FUNCTIONS **/
char*
string_listing(ArrayList l) {
	int i;
	char* bigbuff = malloc(1024);
	bigbuff[0] = '\0';
	strcat(bigbuff, "[");
	for (i = 0; i < arraylist_count(l); i++) {
		char* listStr = arraylist_getitem(l, i);
		strcat(bigbuff, "\"");
		strcat(bigbuff, listStr);
		strcat(bigbuff, "\"");
		if (i < arraylist_count(l) - 1) {
			strcat(bigbuff, ", ");
		}
	}
	strcat(bigbuff, "]");
	return bigbuff;
}

static int8_t
string_comparator(void* a, void* b) {
	/*
	printf("Comparing \"%s\" and \"%s\" => %d\n", (char*)a, (char*)b, strcmp((char*)a, (char*) b));
	*/
	return (int8_t) strcmp((char*)a, (char*)b);
}

static ArrayList
create_string_list(int num_items) {
	int i;
	ArrayList l;
	l = arraylist_create(string_comparator);
	for (i = 0; i < num_items; i++) {
		char* tmp = malloc(sizeof(char) * 10);
		sprintf(tmp, "item %d", i);
		arraylist_append(l, tmp);
	}
	return l;
}

void
print_listing(ArrayList l) {
	int i;
	printf("[");
	for (i = 0; i < arraylist_count(l); i++) {
		printf("\"%s\"", (char*)arraylist_getitem(l, i));
		if (i < arraylist_count(l) - 1) {
			printf(", ");
		}
	}
	printf("]\n");
}

/* arraylist_create */
START_TEST (test_arraylist_create) {
	ArrayList l;
	l = arraylist_create(string_comparator);
	fail_unless(l != NULL, "List was not created");
	fail_unless(arraylist_count(l) == 0, "List should start with count 0");
	fail_unless(l->capacity == 10, "Capacity not 10, update docs");
	arraylist_free(l);
}
END_TEST

/* arraylist_create_heap_size */
START_TEST (test_arraylist_create_heap_size) {
	ArrayList l;
	l = arraylist_create_heap_size(10000, string_comparator);
	fail_unless(l != NULL, "List was not created");
	fail_unless(arraylist_count(l) == 0, "List should have no items");
	fail_unless(l->capacity == 10000, "List has incorrect capacity");
	arraylist_free(l);
}
END_TEST

/* arraylist_create_static */
START_TEST (test_arraylist_create_static) {
	void* buf[1000];
	ArrayList l = arraylist_create_static(buf, 1000, string_comparator);
	fail_unless(l != NULL, "List was not created");
	fail_unless(l->capacity == 1000, "Capacity is not correct");
	fail_unless(arraylist_count(l) == 0, "List should not have any items");
}
END_TEST

/* arraylist_free */
START_TEST (test_arraylist_free) {
	ArrayList l = create_string_list(100);
	fail_unless(arraylist_count(l) == 100, "Items were not added");
	fail_unless(arraylist_free(l) == ARRAYLIST_SUCCESS, "List free failed");
	/* TODO: what is a better way to test this? */
}
END_TEST


/* test arraylist_append and getitem */
START_TEST (test_arraylist_append_getitem) {
	int i;
	char expected[10];
	ArrayList l;
	l = create_string_list(100);
	for (i = 0; i < arraylist_count(l); i++) {
		sprintf(expected, "item %d", i);
		fail_unless(strcmp((char*)arraylist_getitem(l, i), expected) == 0, 
				    "Item Mismatch");
		free(arraylist_getitem(l, i)); /* free up memory */
	}
	arraylist_free(l);
}
END_TEST

/* arraylist_extend */
START_TEST (test_arraylist_extend) {
	int i;
	char expected[10];
	ArrayList list1, list2;
	list1 = create_string_list(10);
	list2 = create_string_list(95);
	arraylist_extend(list1, list2); /* extend list1 by list2 */
	fail_unless(arraylist_count(list1) == 105, "list1 should have 105 items");
	for (i = 0; i < 0; i++) {
		if (i < 10) {
			sprintf(expected, "item %d", i);
		} else {
			sprintf(expected, "item %d", i - 10);
		}
		fail_unless(strcmp((char*)arraylist_getitem(list1, i), expected) == 0, 
					"Item Mismatch");
		free(arraylist_getitem(list1, i));
	}
	arraylist_free(list1);
	arraylist_free(list2);
}
END_TEST

/* arraylist_insert */
START_TEST (test_arraylist_insert) {
	ArrayList l = create_string_list(10);
	char* mystr = "I'm Different";
	fail_unless(arraylist_insert(l, 5, mystr) == ARRAYLIST_SUCCESS);
	fail_unless(arraylist_count(l) == 11, "List count incorrect");
	fail_unless(arraylist_getitem(l, 5) == mystr);
	if (!strcmp(arraylist_getitem(l, 7), "item 6") == 0) {
		char buf[50];
		sprintf(buf, "Expected 'item 6' was '%s'", (char*)arraylist_getitem(l, 7));
		fail(buf);
	}
	arraylist_free(l);
}
END_TEST

/* arraylist_remove */
START_TEST (test_arraylist_remove) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = arraylist_create(string_comparator);
	arraylist_append(l, item1);
	arraylist_append(l, item2);
	arraylist_append(l, item3);
	
	fail_unless(arraylist_count(l) == 3, "Setup Failed");
	fail_if(arraylist_remove(l, item2) == NULL);
	fail_unless(arraylist_count(l) == 2, "Count incorrect after remove");
	fail_unless(arraylist_getitem(l, 0) == item1, string_listing(l));
	fail_unless(arraylist_getitem(l, 1) == item3, string_listing(l));
	
	arraylist_free(l);
}
END_TEST

/* arraylist_pop */
START_TEST (test_arraylist_pop) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = arraylist_create(string_comparator);
	arraylist_append(l, item1);
	arraylist_append(l, item2);
	arraylist_append(l, item3);
	
	fail_unless(arraylist_count(l) == 3, "Setup Failed");
	fail_unless(arraylist_pop(l) == item3);
	fail_unless(arraylist_count(l) == 2, "Count incorrect after remove");
	fail_unless(arraylist_getitem(l, 0) == item1, string_listing(l));
	fail_unless(arraylist_getitem(l, 1) == item2, string_listing(l));
	
	arraylist_free(l);
}
END_TEST

/* arraylist_pop_item */
START_TEST (test_arraylist_pop_item) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = arraylist_create(string_comparator);
	arraylist_append(l, item1);
	arraylist_append(l, item2);
	arraylist_append(l, item3);
	
	fail_unless(arraylist_count(l) == 3, "Setup Failed");
	fail_if(arraylist_pop_item(l, 1) == NULL);
	fail_unless(arraylist_count(l) == 2, "Count incorrect after remove");
	fail_unless(arraylist_getitem(l, 0) == item1, string_listing(l));
	fail_unless(arraylist_getitem(l, 1) == item3, string_listing(l));
	
	arraylist_free(l);
}
END_TEST

/* arraylist_index */
START_TEST (test_arraylist_index) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = arraylist_create(string_comparator);
	arraylist_append(l, item1);
	arraylist_append(l, item2);
	arraylist_append(l, item3);
	
	fail_unless(arraylist_count(l) == 3, "Setup Failed");
	fail_unless(arraylist_index(l, item1) == 0);
	fail_unless(arraylist_contains(l, item1));
	fail_unless(arraylist_index(l, item2) == 1);
	fail_unless(arraylist_contains(l, item2));
	fail_unless(arraylist_index(l, item3) == 2);
	fail_unless(arraylist_contains(l, item3));
	fail_unless(arraylist_index(l, "Foobar") == -1);
	fail_if(arraylist_contains(l, "Foobar"));
	
	arraylist_free(l);
}
END_TEST

/* arraylist_reverse */
START_TEST (test_arraylist_reverse) {
	int i;
	char buf[20];
	ArrayList l = create_string_list(25);
	arraylist_reverse(l);
	fail_unless(arraylist_count(l) == 25, "");
	for (i = 0; i < arraylist_count(l); i++) {
		sprintf(buf, "item %d", 24 - i);
		fail_unless(strcmp(buf, arraylist_getitem(l, i)) == 0, buf);
	}
	arraylist_free(l);
}
END_TEST

/* arraylist_quicksort */
START_TEST (test_arraylist_quicksort) {
	ArrayList l;
	int i;
	char *unordered_words[] = {
		"yellow",
		"green",
		"blue",
		"orange",
		"pink",
		"purple",
		"lavender",
		"black",
		"red",
	};
	
	char *ordered_words[] = {
		"black",
		"blue",
		"green",
		"lavender",
		"orange",
		"pink",
		"purple",
		"red",
		"yellow",
	};
	
	/* create the list and add unordered words */
	l = arraylist_create(string_comparator);
	for (i = 0; i < 9; i++) {
		arraylist_append(l, unordered_words[i]);
	}
	
	arraylist_sort(l);
	for (i = 0; i < 9; i++) {
		fail_unless(strcmp((char*)arraylist_getitem(l, i), ordered_words[i]) == 0);
	}
}
END_TEST

Suite*
arraylist_suite(void) {
	Suite *s = suite_create("List");
	
	/* Core test case */
	TCase *tc_core = tcase_create("List");
	tcase_add_test(tc_core, test_arraylist_create);
	tcase_add_test(tc_core, test_arraylist_create_heap_size);
	tcase_add_test(tc_core, test_arraylist_create_static);
	tcase_add_test(tc_core, test_arraylist_free);
	tcase_add_test(tc_core, test_arraylist_append_getitem);
	tcase_add_test(tc_core, test_arraylist_extend);
	tcase_add_test(tc_core, test_arraylist_insert);
	tcase_add_test(tc_core, test_arraylist_remove);
	tcase_add_test(tc_core, test_arraylist_pop);
	tcase_add_test(tc_core, test_arraylist_pop_item);
	tcase_add_test(tc_core, test_arraylist_index);
	tcase_add_test(tc_core, test_arraylist_reverse);
	tcase_add_test(tc_core, test_arraylist_quicksort);
	
	suite_add_tcase(s, tc_core);
	return s;
}
