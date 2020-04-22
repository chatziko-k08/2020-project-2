//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT List.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h" // Απλή βιβλιοθήκη για unit testing

#include "ADTBList.h"

void test_create(void) {
	// Δημιουργούμε μια κενή λίστα με NULL δείκτη συνάρτησης delete_value
	BList list = blist_create(NULL);
	blist_set_destroy_value(list, NULL);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_ASSERT(list != NULL);
	TEST_ASSERT(blist_size(list) == 0);

	blist_destroy(list);
}

void test_insert(void) {
	BList list = blist_create(NULL);

	// Θα προσθέτουμε, μέσω της insert, δείκτες ως προς τα στοιχεία του π΄ίνακα
	int N = 1000;
	int array[N];

	for (int i = 0; i < N; i++) {
		blist_insert(list, blist_first(list), &array[i]);

		// Ελέγχουμε εάν ενημερώθηκε (αυξήθηκε) το μέγεθος της λίστας.
		TEST_ASSERT(blist_size(list) == (i + 1));

		// Ελέγχουμε εάν ο πρώτος κόμβος περιέχει σαν τιμή τον δείκτη που μόλις κάναμε insert
		TEST_ASSERT(blist_node_value(list, blist_first(list)) == &array[i]);
	}

	// Ελέγχουμε εάν τα στοιχεία έχουν μπει με την αντίστροφη σειρά
	BListNode node = blist_first(list);

	for (int i = N - 1; i >= 0; i--) {
		TEST_ASSERT(blist_node_value(list, node) == &array[i]);
		node = blist_next(list, node);
	}

	// Εισαγωγή σε ενδιάμεσο κόμβο: προσθέτουμε το NULL σαν πρωτο κόμβο
	BListNode first_node = blist_first(list);
	blist_insert(list, first_node, NULL);
	TEST_ASSERT(blist_node_value(list, blist_previous(list, first_node)) == NULL);

	blist_destroy(list);
}

void test_remove_next(void) {
	// Δημιουργία λίστας που καλεί αυτόματα τη free σε κάθε στοιχείο που αφαιρείται
	BList blist = blist_create(free);

	int N = 1000;
	int *array[N];

	// Χρησιμοποιούμε την insert για να γεμίσουμε την λίστα, αφού την έχουμε δοκιμάσει ήδη στην test_insert()
	for (int i = 0; i < N; i++) {
		// Δημιουργούμε δυναμικά δεσμευμένα αντικείμενα για να δοκιμάσουμε την destroy_function
		array[i] = malloc(sizeof(int));
		*array[i] = i;
		blist_insert(blist, blist_first(blist), array[i]);
	}

	for (int i = N - 1; i >= 0; i--) {
		// Διαγράφουμε απο την αρχή και ελέγχουμε εάν η τιμή του πρώτου κόμβου
		// ήταν η ίδια με αυτή που κάναμε insert παραπάνω
		TEST_ASSERT(blist_node_value(blist, blist_first(blist)) == array[i]);
		blist_remove(blist, blist_first(blist));

		// Ελέγχουμε ότι ενημερώνεται (μειώνεται) το size/μέγεθος της λίστας
		TEST_ASSERT(blist_size(blist) == i);
	}

	// Ξαναγεμίζουμε την λίστα για να δοκιμάσουμε την διαγραφή απο ενδιάμεσο κόμβο
	for (int i = 0; i < N; i++) {
		array[i] = malloc(sizeof(int));
		*array[i] = i;
		blist_insert(blist, blist_first(blist), array[i]);
	}

	// Δοκιμάζουμε την διαγραφή κόμβων ενδιάμεσα της λίστας, και συγκεκριμένα του δεύτερου κόμβου απο την αρχή
	blist_remove(blist, blist_first(blist));
	TEST_ASSERT(blist_size(blist) == N - 1);

	blist_destroy(blist);
}

// Σύγκριση δύο int pointers
int compare_ints(Pointer a, Pointer b) {
	return *(int *)a - *(int *)b;
}

void test_find() {
	BList blist = blist_create(NULL);
	int N = 1000;
	int array[N];

	// Εισάγουμε δοκιμαστικές τιμές στον πίνακα, για να ελέγξουμε την test_find
	for (int i = 0; i < N; i++) {
		array[i] = i;
		blist_insert(blist, BLIST_BOF, &array[i]);
	}

	// Εύρεση όλων των στοιχείων
	for (int i = 0; i < N; i++) {
		int *value = blist_find(blist, &i, compare_ints);
		TEST_ASSERT(value == &array[i]);
	}

	// Δοκιμάζουμε, για μια τυχαία τιμή που δεν μπορεί πιθανώς να υπάρχει στην λίστα,
	// αν η blist_find γυρνάει σωστά NULL pointer
	int not_exists = -1;
	TEST_ASSERT(blist_find(blist, &not_exists, compare_ints) == NULL);

	blist_destroy(blist);
}

void test_find_node() {
	BList blist = blist_create(NULL);

	// Εισαγωγή τιμών στον πίνακα
	int N = 1000;
	int array[N];

	for (int i = 0; i < N; i++) {
		array[i] = i;
		blist_insert(blist, blist_first(blist), &array[i]);
	}

	// Ξεκινάμε από την αρχή της λίστας
	BListNode node = blist_first(blist);

	for (int i = N - 1; i >= 0; i--) {
		// Ελέγχουμε ότι η blist_find_node βρίσκει σωστά τον πρώτο κόμβο με value τον δείκτη &array[i].
		// Σε αυτή την λίστα, δοκιμάζουμε ότι ο πρώτος κόμβος περιέχει τον δείκτη &array[N - 1],
		// o δεύτερος τον &array[998] κοκ
		BListNode found_node = blist_find_node(blist, &i, compare_ints);
		TEST_ASSERT(found_node == node);
		TEST_ASSERT(blist_node_value(blist, found_node) == &array[i]);

		// Προχωράμε στον επόμενο κόμβο για να προσπελάσουμε όλη την λίστα
		node = blist_next(blist, node);
	}

	blist_destroy(blist);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{"blist_create", test_create},
	{"blist_insert", test_insert},
	{"blist_remove", test_remove_next},
	{"blist_find", test_find},
	{"blist_find_node", test_find_node},
	{NULL, NULL} // τερματίζουμε τη λίστα με NULL
};