/*
 * test_datastructures.c
 *
 *  Created on: Jan 18, 2010
 *      Author: Paul Osborne <osbpau@gmail.com>
 */
#include <check.h>
#include "tests.h"

#if 1
int
main(void) {
	int number_failed;
	SRunner *sr = srunner_create(arraylist_suite());
	srunner_add_suite(sr, deque_suite());
	
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? 0 : 1;
}

#else
#include "../src/deque.h"
#include <assert.h>


int8_t
string_comp(const void *a, const void *b) {
	return strcmp((char*) a, (char*) b);
}

int main(void) {
	char* test_strings[] = {"t1", "t2", "t3"};
	Deque d = deque_create(string_comp);
	
	assert(deque_count(d) == 0);
	
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	
	assert(deque_count(d) == 3);
	assert(deque_contains(d, test_strings[0]));
	assert(deque_contains(d, test_strings[1]));
	assert(deque_contains(d, test_strings[2]));
	assert(!deque_contains(d, "bar"));
	assert(!deque_contains(d, "foo"));
	
	deque_free(d);
}
#endif
