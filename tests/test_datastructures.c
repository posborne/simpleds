/*
 * test_datastructures.c
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
	srunner_add_suite(sr, redblack_suite());
	
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
