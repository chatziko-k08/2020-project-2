//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT BList.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTBList.h"


void test_create(void) {
	// ...
	TEST_ASSERT(0);
}

void test_insert(void) {
	TEST_ASSERT(0);
}

void test_remove(void) {
	TEST_ASSERT(0);
}

void test_find() {
	TEST_ASSERT(0);
}

void test_find_node() {
	TEST_ASSERT(0);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "list_create", test_create },
	{ "list_insert_next", test_insert },
	{ "list_remove", test_remove },
	{ "list_find", test_find },
	{ "list_find_node", test_find_node },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};