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

int main(void) {
	Deque d = deque_create();
	char* ts[] = {"t1", "t2", "t3"};
	deque_append(d, ts[0]);
	deque_append(d, ts[1]);
	deque_append(d, ts[2]);
	
	/* slide to the right */
	assert(deque_peek(d) == ts[2]);
	deque_rotate(d, 1);
	assert(deque_peek(d) == ts[1]);
	
	/* slide to the left */
	deque_rotate(d, -1);
	assert(deque_peek(d) == ts[2]);
	
	/* slide two to the right */
	deque_rotate(d, 2);
	assert(deque_peek(d) == ts[0]);
	
	/* slide two again */
	deque_rotate(d, 2);
	assert(deque_peek(d) == ts[1]);
}
#endif
