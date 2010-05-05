/* 
 * test_deque.c
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
#include <string.h>
#include "../src/deque.h"

int8_t
string_comparator(const void *a, const void *b) {
	return strcmp((char*) a, (char*) b);
}

START_TEST (test_deque_create) {
	Deque d = deque_create(string_comparator);
	fail_if(d == NULL);
	fail_unless(d->number_items == 0);
	fail_unless(d->head == NULL);
	fail_unless(d->tail == NULL);
	deque_free(d);
}
END_TEST


START_TEST (test_deque_append) {
	Deque d = deque_create(string_comparator);
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
	Deque d = deque_create(string_comparator);
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
	Deque d = deque_create(string_comparator);
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
	Deque d = deque_create(string_comparator);
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
	Deque d = deque_create(string_comparator);
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
	Deque d = deque_create(string_comparator);
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
	Deque d = deque_create(string_comparator);
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
	Deque d = deque_create(string_comparator);
	
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
	Deque d = deque_create(string_comparator);
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
	char* test_strings[] = {"t1", "t2", "t3", "t2"};
	Deque d = deque_create(string_comparator);
        
	fail_unless(deque_count(d) == 0);
	
	deque_append(d, test_strings[0]);
	deque_append(d, test_strings[1]);
	deque_append(d, test_strings[2]);
	deque_append(d, test_strings[3]);
	
	fail_unless(deque_count(d) == 4);
	fail_unless(deque_peek(d) == test_strings[3]);
	fail_unless(deque_peekleft(d) == test_strings[0]);
	
	/* do the reverse */
	deque_reverse(d);
	
	fail_unless(deque_count(d) == 4);
	fail_unless(deque_pop(d) == test_strings[0]);
	fail_unless(deque_pop(d) == test_strings[1]);
	fail_unless(deque_pop(d) == test_strings[2]);
	fail_unless(deque_pop(d) == test_strings[3]);
}
END_TEST

START_TEST (test_deque_contains) {
	char* test_strings[] = {"t1", "t2", "t3"};
	Deque d = deque_create(string_comparator);
	
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

