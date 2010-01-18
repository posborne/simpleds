/*
 * test_list.c
 *
 *  Created on: Jan 16, 2010
 *      Author: Paul Osborne <osbpau@gmail.com>
 */

#include <check.h>
#include <malloc.h>
#include <stdio.h>
#include "../src/list.h"

/** TEST UTILITY FUNCTIONS **/
static ArrayList
create_string_list(int num_items) {
	int i;
	ArrayList l;
	l = list_create();
	for (i = 0; i < num_items; i++) {
		char* tmp = malloc(sizeof(char) * 10);
		sprintf(tmp, "item %d", i);
		list_append(l, tmp);
	}
	return l;
}

char*
string_listing(ArrayList l) {
	int i;
	char* bigbuff = malloc(1024);
	bigbuff[0] = '\0';
	strcat(bigbuff, "[");
	for (i = 0; i < list_count(l); i++) {
		char* listStr = list_getitem(l, i);
		strcat(bigbuff, "\"");
		strcat(bigbuff, listStr);
		strcat(bigbuff, "\"");
		if (i < list_count(l) - 1) {
			strcat(bigbuff, ", ");
		}
	}
	strcat(bigbuff, "]");
	return bigbuff;
}

void
print_listing(ArrayList l) {
	int i;
	printf("[");
	for (i = 0; i < list_count(l); i++) {
		printf("\"%s\"", (char*)list_getitem(l, i));
		if (i < list_count(l) - 1) {
			printf(", ");
		}
	}
	printf("]\n");
}

/* list_create */
START_TEST (test_list_create) {
	ArrayList l;
	l = list_create();
	fail_unless(l != NULL, "List was not created");
	fail_unless(list_count(l) == 0, "List should start with count 0");
	fail_unless(l->capacity == 10, "Capacity not 10, update docs");
	list_free(l);
}
END_TEST

/* list_create_heap_size */
START_TEST (test_list_create_heap_size) {
	ArrayList l;
	l = list_create_heap_size(10000);
	fail_unless(l != NULL, "List was not created");
	fail_unless(list_count(l) == 0, "List should have no items");
	fail_unless(l->capacity == 10000, "List has incorrect capacity");
	list_free(l);
}
END_TEST

/* list_create_static */
START_TEST (test_list_create_static) {
	void* buf[1000];
	ArrayList l = list_create_static(buf, 1000);
	fail_unless(l != NULL, "List was not created");
	fail_unless(l->capacity == 1000, "Capacity is not correct");
	fail_unless(list_count(l) == 0, "List should not have any items");
}
END_TEST

/* list_free */
START_TEST (test_list_free) {
	ArrayList l = create_string_list(100);
	fail_unless(list_count(l) == 100, "Items were not added");
	fail_unless(list_free(l) == LIST_SUCCESS, "List free failed");
	/* TODO: what is a better way to test this? */
}
END_TEST


/* test list_append and getitem */
START_TEST (test_list_append_getitem) {
	int i;
	char expected[10];
	ArrayList l;
	l = create_string_list(100);
	for (i = 0; i < list_count(l); i++) {
		sprintf(expected, "item %d", i);
		fail_unless(strcmp((char*)list_getitem(l, i), expected) == 0, 
				    "Item Mismatch");
		free(list_getitem(l, i)); /* free up memory */
	}
	list_free(l);
}
END_TEST

/* list_extend */
START_TEST (test_list_extend) {
	int i;
	char expected[10];
	ArrayList list1, list2;
	list1 = create_string_list(10);
	list2 = create_string_list(95);
	list_extend(list1, list2); /* extend list1 by list2 */
	fail_unless(list_count(list1) == 105, "list1 should have 105 items");
	for (i = 0; i < 0; i++) {
		if (i < 10) {
			sprintf(expected, "item %d", i);
		} else {
			sprintf(expected, "item %d", i - 10);
		}
		fail_unless(strcmp((char*)list_getitem(list1, i), expected) == 0, 
					"Item Mismatch");
		free(list_getitem(list1, i));
	}
	list_free(list1);
	list_free(list2);
}
END_TEST

/* list_insert */
START_TEST (test_list_insert) {
	ArrayList l = create_string_list(10);
	char* mystr = "I'm Different";
	fail_unless(list_insert(l, 5, mystr) == LIST_SUCCESS);
	fail_unless(list_count(l) == 11, "List count incorrect");
	fail_unless(list_getitem(l, 5) == mystr);
	if (!strcmp(list_getitem(l, 7), "item 6") == 0) {
		char buf[50];
		sprintf(buf, "Expected 'item 6' was '%s'", (char*)list_getitem(l, 7));
		fail(buf);
	}
	list_free(l);
}
END_TEST

/* list_remove */
START_TEST (test_list_remove) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = list_create();
	list_append(l, item1);
	list_append(l, item2);
	list_append(l, item3);
	
	fail_unless(list_count(l) == 3, "Setup Failed");
	fail_if(list_remove(l, item2) == NULL);
	fail_unless(list_count(l) == 2, "Count incorrect after remove");
	fail_unless(list_getitem(l, 0) == item1, string_listing(l));
	fail_unless(list_getitem(l, 1) == item3, string_listing(l));
	
	list_free(l);
}
END_TEST

/* list_pop */
START_TEST (test_list_pop) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = list_create();
	list_append(l, item1);
	list_append(l, item2);
	list_append(l, item3);
	
	fail_unless(list_count(l) == 3, "Setup Failed");
	fail_unless(list_pop(l) == item3);
	fail_unless(list_count(l) == 2, "Count incorrect after remove");
	fail_unless(list_getitem(l, 0) == item1, string_listing(l));
	fail_unless(list_getitem(l, 1) == item2, string_listing(l));
	
	list_free(l);
}
END_TEST

/* list_pop_item */
START_TEST (test_list_pop_item) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = list_create();
	list_append(l, item1);
	list_append(l, item2);
	list_append(l, item3);
	
	fail_unless(list_count(l) == 3, "Setup Failed");
	fail_if(list_pop_item(l, 1) == NULL);
	fail_unless(list_count(l) == 2, "Count incorrect after remove");
	fail_unless(list_getitem(l, 0) == item1, string_listing(l));
	fail_unless(list_getitem(l, 1) == item3, string_listing(l));
	
	list_free(l);
}
END_TEST

/* list_index */
START_TEST (test_list_index) {
	char item1[] = "item 1";
	char item2[] = "item 2";
	char item3[] = "item 3";
	ArrayList l = list_create();
	list_append(l, item1);
	list_append(l, item2);
	list_append(l, item3);
	
	fail_unless(list_count(l) == 3, "Setup Failed");
	fail_unless(list_index(l, item1) == 0);
	fail_unless(list_index(l, item2) == 1);
	fail_unless(list_index(l, item3) == 2);
	fail_unless(list_index(l, "Foobar") == -1);
	
	list_free(l);
}
END_TEST

/* list_reverse */
START_TEST (test_list_reverse) {
	int i;
	char buf[20];
	ArrayList l = create_string_list(25);
	list_reverse(l);
	fail_unless(list_count(l) == 25, "");
	for (i = 0; i < list_count(l); i++) {
		sprintf(buf, "item %d", 24 - i);
		fail_unless(strcmp(buf, list_getitem(l, i)) == 0, buf);
	}
	list_free(l);
}
END_TEST

static int8_t
string_comparator(void* a, void* b) {
	// printf("Comparing \"%s\" and \"%s\" => %d\n", (char*)a, (char*)b, strcmp((char*)a, (char*) b));
	return (int8_t) strcmp((char*)a, (char*)b);
}

/* list_quicksort */
START_TEST (test_list_quicksort) {
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
	l = list_create();
	for (i = 0; i < 9; i++) {
		list_append(l, unordered_words[i]);
	}
	
	list_sort(l, &string_comparator);
	for (i = 0; i < 9; i++) {
		fail_unless(strcmp((char*)list_getitem(l, i), ordered_words[i]) == 0);
	}
}
END_TEST

Suite*
list_suite(void) {
	Suite *s = suite_create("List");
	
	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_list_create);
	tcase_add_test(tc_core, test_list_create_heap_size);
	tcase_add_test(tc_core, test_list_create_static);
	tcase_add_test(tc_core, test_list_free);
	tcase_add_test(tc_core, test_list_append_getitem);
	tcase_add_test(tc_core, test_list_extend);
	tcase_add_test(tc_core, test_list_insert);
	tcase_add_test(tc_core, test_list_remove);
	tcase_add_test(tc_core, test_list_pop);
	tcase_add_test(tc_core, test_list_pop_item);
	tcase_add_test(tc_core, test_list_index);
	tcase_add_test(tc_core, test_list_reverse);
	tcase_add_test(tc_core, test_list_quicksort);
	
	suite_add_tcase(s, tc_core);
	return s;
}

int
main(void) {
	int number_failed;
	Suite *s = list_suite();
	SRunner *sr = srunner_create(s);
	
	srunner_run_all(sr, CK_NOFORK);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? 0 : 1;
}
