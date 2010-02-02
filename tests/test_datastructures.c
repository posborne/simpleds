/*
 * test_datastructures.c
 *
 *  Created on: Jan 18, 2010
 *      Author: Paul Osborne <osbpau@gmail.com>
 */
#include <check.h>
#include "tests.h"

int
main(void) {
	int number_failed;
	SRunner *sr = srunner_create(arraylist_suite());
	srunner_add_suite(sr, deque_suite());
	
	srunner_run_all(sr, CK_NOFORK);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? 0 : 1;
}
