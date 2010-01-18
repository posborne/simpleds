#include <check.h>
#include "../src/deque.h"

START_TEST (test_deque_create) {
	fail("Not Implemented");
}
END_TEST


START_TEST (test_deque_append) {
	fail("Not Implemented");
}
END_TEST


START_TEST (test_deque_appendleft) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_clear) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_pop) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_popleft) {
	fail("Not Implemented");	
}
END_TEST

START_TEST (test_deque_remove) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_rotate) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_count) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_copy) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_reverse) {
	fail("Not Implemented");
}
END_TEST

START_TEST (test_deque_contains) {
	fail("Not Implemented");
}
END_TEST

Suite*
deque_suite(void) {
	Suite *s = suite_create("Deque");
	
	/* Core test case */
	TCase *tc_core = tcase_create("Deque");
	tcase_add_test(tc_core, test_deque_create);
	tcase_add_test(tc_core, test_deque_append);
	tcase_add_test(tc_core, test_deque_appendleft);
	tcase_add_test(tc_core, test_deque_clear);
	tcase_add_test(tc_core, test_deque_pop);
	tcase_add_test(tc_core, test_deque_popleft);
	tcase_add_test(tc_core, test_deque_remove);
	tcase_add_test(tc_core, test_deque_rotate);
	tcase_add_test(tc_core, test_deque_count);
	tcase_add_test(tc_core, test_deque_copy);
	tcase_add_test(tc_core, test_deque_reverse);
	tcase_add_test(tc_core, test_deque_contains);
	
	suite_add_tcase(s, tc_core);
	return s;
}

