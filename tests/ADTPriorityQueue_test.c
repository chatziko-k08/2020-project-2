//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Priority Queue.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTPriorityQueue.h"

// θέτει τα στοιχεία του πίνακα array  σε τυχαία σειρά
void shuffle(int* array[], int size) {
	for (int i = 0; i < size; i++) {
		int new_pos = i + rand() / (RAND_MAX / (size - i) + 1);
		int* temp = array[new_pos];
		array[new_pos] = array[i];
		array[i] = temp;
	}
}

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void test_create(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, NULL, NULL);
	pqueue_set_destroy_value(pqueue, NULL);

	TEST_ASSERT(pqueue != NULL);
	TEST_ASSERT(pqueue_size(pqueue) == 0);

	pqueue_destroy(pqueue);

	// create με αρχικά στοιχεία
	Vector values = vector_create(0, NULL);			// χωρίς destroy function, το destroy θα το κάνει η ουρά!
	vector_insert_last(values, create_int(0));
	vector_insert_last(values, create_int(1));

	pqueue = pqueue_create(compare_ints, free, values);
	TEST_ASSERT(pqueue != NULL);
	TEST_ASSERT(pqueue_size(pqueue) == 2);

	TEST_ASSERT(*(int*)pqueue_max(pqueue) == 1);
	pqueue_remove_max(pqueue);
	TEST_ASSERT(*(int*)pqueue_max(pqueue) == 0);

	vector_destroy(values);
	pqueue_destroy(pqueue);
}	

void test_insert(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, NULL, NULL);
	int N = 1000;
	int array[N];					// Στο pqueue θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert N στοιχεία
	for (int i = 0; i < N; i++) {
		array[i] = i;
		pqueue_insert(pqueue, &array[i]);
		TEST_ASSERT(pqueue_size(pqueue) == i+1);			// Το size ενημερώθηκε;
		TEST_ASSERT(pqueue_max(pqueue) == &array[i]);	// Εισαγωγή σε αύξουσα σειρά, το στοιχείο που μόλις βάλαμε πρέπει να είναι στην κορυφή
	}

	pqueue_destroy(pqueue);
}

void test_remove_max(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);

	// προσθήκη δεδομένων, τυχαία σειρά
	int N = 10;
	int* array[N];
	for (int i = 0; i < N; i++)
		array[i] = create_int(i);
	shuffle(array, N);

	for (int i = 0; i < N; i++)
		pqueue_insert(pqueue, array[i]);

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for (int i = N-1; i >= 0; i--) {
		int* value = pqueue_max(pqueue);
		TEST_ASSERT(*value == i);
		TEST_ASSERT(pqueue_max(pqueue) == value);
		pqueue_remove_max(pqueue);
		TEST_ASSERT(pqueue_size(pqueue) == i);
	}

	pqueue_destroy(pqueue);

	// remove από ουρά χωρίς συνάρτηση destroy
	pqueue = pqueue_create(compare_ints, NULL, NULL);
	pqueue_insert(pqueue, &N);
	TEST_ASSERT(pqueue_max(pqueue) == &N);
	pqueue_remove_max(pqueue);
	pqueue_destroy(pqueue);
}

void test_remove_node(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);

	// προσθήκη δεδομένων, αποθηκεύουμε τους κόμβους
	int N = 10;
	PriorityQueueNode nodes[N];

	for (int i = 0; i < N; i++)
		nodes[i] = pqueue_insert(pqueue, create_int(i));

	// remove τους άρτιους με τη σειρά
	int current_size = N;
	for (int i = 0; i < N; i += 2) {
		pqueue_remove_node(pqueue, nodes[i]);

		// Πρέπει να αλλάζει το μέγεθος αλλά όχι το max (είναι N-1, μεγαλύτερο από τους αριθμούς που αφαιρούνται)
		TEST_ASSERT(*(int*)pqueue_max(pqueue) == N-1);
		TEST_ASSERT(pqueue_size(pqueue) == --current_size);
	}

	// remove τους περιττούς με αντίστροφη σειρά
	for (int i = N-1; i >= 0; i -= 2) {
		// το max κάθε φορά πρέπει να αλλάζει
		TEST_ASSERT(*(int*)pqueue_max(pqueue) == i);

		// έλεγχος ότι παρά τα removes, οι κόμβοι έχουν ακόμα τις σωστές τιμές
		TEST_ASSERT(*(int*)pqueue_node_value(pqueue, nodes[i]) == i);

		pqueue_remove_node(pqueue, nodes[i]);
		TEST_ASSERT(pqueue_size(pqueue) == --current_size);
	}

	pqueue_destroy(pqueue);
}

void test_update_order(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);

	// προσθήκη δεδομένων, αποθηκεύουμε τους κόμβους
	int N = 10;
	PriorityQueueNode nodes[N];

	for (int i = 0; i < N; i++)
		nodes[i] = pqueue_insert(pqueue, create_int(i));

	// Αντικατάσταση κάθε i με -i, με αντίστροφη σειρά, και update order.
	// Το max αλλάζει κάθε φορά
	for (int i = N-1; i > 0; i--) {
		*(int*)pqueue_node_value(pqueue, nodes[i]) *= -1;	// Αλλαγή του περιεχομένου του value
		pqueue_update_order(pqueue, nodes[i]);

		// Ελεγχος ότι το max άλλαξε (αλλά το μέγεθος όχι)
		TEST_ASSERT(*(int*)pqueue_max(pqueue) == i-1);
		TEST_ASSERT(pqueue_size(pqueue) == N);
	}

	// Διαδοχικά remove_max, πρέπει να πάρουμε τους αριθμούς 0 .. -(Ν-1)
	for (int i = 0; i < N; i++) {
		TEST_ASSERT(*(int*)pqueue_max(pqueue) == -i);
		pqueue_remove_max(pqueue);
		TEST_ASSERT(pqueue_size(pqueue) == N-i-1);
	}

	pqueue_destroy(pqueue);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "pqueue_create", test_create },
	{ "pqueue_insert", test_insert },
	{ "pqueue_remove_max", test_remove_max },
	{ "pqueue_remove_node", test_remove_node },
	{ "pqueue_update_order", test_update_order },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};