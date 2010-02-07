#include <check.h>
#include "../src/deque.h"

START_TEST (test_deque_create) {
	Deque d = deque_create();
	fail_if(d == NULL);
	fail_unless(d->number_items == 0);
	fail_unless(d->head == NULL);
	fail_unless(d->tail == NULL);
	deque_free(d);
}
END_TEST


START_TEST (test_deque_append) {
	Deque d = deque_create();
	char* test_strings[] = {"t1", "t2", "t3"};
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	fail_unless(test_strings[2] == deque_pop(d));
	fail_unless(test_strings[1] == deque_pop(d));
	fail_unless(test_strings[0] == deque_pop(d));
	deque_free(d);
}
END_TEST


START_TEST (test_deque_appendleft) {
	Deque d = deque_create();
	char* test_strings[] = {"t1", "t2", "t3"};
	deque_appendleft(d, test_strings[0]);
	deque_appendleft(d, test_strings[1]);
	deque_appendleft(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	fail_unless(test_strings[0] == deque_pop(d));
	fail_unless(test_strings[1] == deque_pop(d));
	fail_unless(test_strings[2] == deque_pop(d));
	deque_free(d);
}
END_TEST

START_TEST (test_deque_clear) {
	Deque d = deque_create();
	char* test_strings[] = {"t1", "t2", "t3"};
	deque_appendleft(d, test_strings[0]);
	deque_appendleft(d, test_strings[1]);
	deque_appendleft(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	deque_clear(d);
	fail_unless(deque_count(d) == 0);
	fail_unless(d->head == NULL);
	fail_unless(d->tail == NULL);
	deque_free(d);
}
END_TEST

START_TEST (test_deque_pop) {
	Deque d = deque_create();
	char* test_strings[] = {"t1", "t2", "t3"};
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	fail_unless(test_strings[2] == deque_pop(d));
	fail_unless(test_strings[1] == deque_pop(d));
	fail_unless(test_strings[0] == deque_pop(d));
	deque_free(d);
}
END_TEST

START_TEST (test_deque_popleft) {
	Deque d = deque_create();
	char* test_strings[] = {"t1", "t2", "t3"};
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	fail_unless(test_strings[0] == deque_popleft(d));
	fail_unless(test_strings[1] == deque_popleft(d));
	fail_unless(test_strings[2] == deque_popleft(d));
}
END_TEST

START_TEST (test_deque_remove) {
	Deque d = deque_create();
	char* test_strings[] = {"t1", "t2", "t3"};
	char* notfound = "Not in there";
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	fail_unless(deque_remove(d, notfound) == NULL);
	fail_unless(deque_count(d) == 3);
	
	fail_unless(deque_remove(d, test_strings[1]) == test_strings[1]);
	fail_unless(deque_count(d) == 2);
	
	fail_unless(deque_remove(d, test_strings[2]) == test_strings[2]);
	fail_unless(deque_count(d) == 1);
}
END_TEST

START_TEST (test_deque_rotate) {
	Deque d = deque_create();
	char* ts[] = {"t1", "t2", "t3"};
	deque_append(d, ts[0]);
	deque_append(d, ts[1]);
	deque_append(d, ts[2]);
	
	/* slide to the right */
	fail_unless(deque_peek(d) == ts[2]);
	deque_rotate(d, 1);
	fail_unless(deque_peek(d) == ts[1]);
	
	/* slide to the left */
	deque_rotate(d, -1);
	fail_unless(deque_peek(d) == ts[2]);
	
	/* slide two to the right */
	deque_rotate(d, 2);
	fail_unless(deque_peek(d) == ts[0]);
	
	/* slide two again */
	deque_rotate(d, 2);
	fail_unless(deque_peek(d) == ts[1]);
}
END_TEST

START_TEST (test_deque_count) {
	char* test_strings[] = {"t1", "t2", "t3"};
	Deque d = deque_create();
	
	fail_unless(deque_count(d) == 0);
	
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	deque_free(d);
}
END_TEST

START_TEST (test_deque_copy) {
	Deque dcopy;
	Deque d = deque_create();
	char* test_strings[] = {"t1", "t2", "t3"};
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);

	dcopy = deque_copy(d);
	fail_unless(deque_count(d) == 3);
	
	deque_free(dcopy);
	deque_free(d);
}
END_TEST

START_TEST (test_deque_reverse) {
	char* test_strings[] = {"t1", "t2", "t3"};
		Deque d = deque_create();
		
		fail_unless(deque_count(d) == 0);
		
		deque_append(d, test_strings[0]);
		deque_append(d, test_strings[1]);
		deque_append(d, test_strings[2]);
		
		fail_unless(deque_count(d) == 3);

		/* do the reverse */
		deque_reverse(d);
		
		fail_unless(deque_count(d) == 3);
		fail_unless(deque_peek(d) == test_strings[0]);
		fail_unless(deque_peekleft(d) == test_strings[2]);
				
}
END_TEST

START_TEST (test_deque_contains) {
	char* test_strings[] = {"t1", "t2", "t3"};
	Deque d = deque_create();
	
	fail_unless(deque_count(d) == 0);
	
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	
	fail_unless(deque_count(d) == 3);
	fail_unless(deque_contains(d, test_strings[0]));
	fail_unless(deque_contains(d, test_strings[1]));
	fail_unless(deque_contains(d, test_strings[2]));
	fail_if(deque_contains(d, "bar"));
	fail_if(deque_contains(d, "foo"));
	
	deque_free(d);
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

