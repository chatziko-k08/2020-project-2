///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT BList μέσω διπλά συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTBList.h"



// Ενα BList είναι pointer σε αυτό το struct
struct blist {
	BListNode first;			//Ο πρώτος κόμβος της BList
	BListNode last;				//Ο τελευταίος κόμβος της BList
	int size;				   // μέγεθος, ώστε η list_size να είναι Ο(1)
	DestroyFunc destroy_value; // Συνάρτηση που καταστρέφει ένα στοιχείο της λίστας.
};

struct blist_node {
	Pointer value;
	BListNode next;
	BListNode prev;
};


BList blist_create(DestroyFunc destroy_value) {

	BList list = malloc(sizeof(*list));

	//Αρχικοποιούμε τις τιμές των πεδίων του header BList
	list->destroy_value = destroy_value;
	
	list->first = BLIST_BOF;
	list->last = BLIST_EOF;
	
	list->size = 0;

	return list;
}

int blist_size(BList blist) {
	assert(blist != NULL);
	return blist->size;
}

void blist_insert(BList blist, BListNode node, Pointer value) {
	//Φτιάχνουμε τον καινούργιο κόμβο
	BListNode new = malloc(sizeof(*new));
	new->value = value;
	
	//Αν το node είναι το BLIST_EOF τότε προσθέτουμε τον κόμβο στο τέλος
	if (node == BLIST_EOF) {
		new->next = BLIST_EOF;
		new->prev = blist->last;
		
		//Σε περίπτωση που η λίστα είναι εντελώς κενή τότε πρέπει να θέσουμε και το blist->first
		if (blist->first == BLIST_BOF) 
			blist->first = new;
		
		//Πρέπει να ελεγχουμε αν υπάρχει last αλλιώς θα προσπαθήσουμε να διαβάσουμε πεδίο απο NULL κόμβο
		if (blist->last != BLIST_EOF)
			blist->last->next = new;

		//Θέτουμε την καινούργια ουρά να είναι το new node
		blist->last = new;

		blist->size++;
	} else {
		BListNode prev = node->prev;
		
		//Συνδέουμε το node με το new
		node->prev = new;

		//και το new με το node
		new->next = node;

		//τώρα πρέπει να συνδέσουμε και το prev που αποθηκεύσαμε με το new
		if (prev != NULL)
			prev->next = new;
		
		new->prev = prev;

		if (node == blist->first)
			blist->first = new;

		blist->size++;
	}
}

void blist_remove(BList blist, BListNode node) {
	assert(blist != NULL);
	//Αν ο χρήστης περάσει για τιμή του node την τιμή NULL τότε θα διαγράψουμε τον τελευταίο κόμβο.
	if (node == NULL)
		node = blist->last;
	//Αν ο node δεν είναι NULL και δεν είναι ούτε ο blist->last τότε
	if (node) {
		BListNode prev, next;
		//Θέτουμε τους 2 pointers για να εξηγήσουμε πιο αναλυτικά την διαδικασία
		prev = node->prev;
		next = node->next;

		//Τώρα πρέπει να συνδέσουμε τα prev και next μεταξυ τους
		if (prev != NULL)
			prev->next = next;
		if (next != NULL)
			next->prev = prev;

		//Αν ο node είναι το blist->first τότε θέτουμε το next ως το καινούργιο blist->first
		if (node == blist->first)
			blist->first = next;
		//Αντίστοιχα αν είναι το blist->last θέτουμε το prev ως νέο blist->last
		if (node == blist->last)
			blist->last = prev;

		//Τώρα μένει μόνο να καταστρέψουμε τον κόμβο
		if (blist->destroy_value != NULL)
			blist->destroy_value(node->value);
		free(node);
		blist->size--;
	}
	
}

Pointer blist_find(BList blist, Pointer value, CompareFunc compare) {
	BListNode node = blist_find_node(blist, value, compare);
	return node == NULL ? NULL : node->value;
}

DestroyFunc blist_set_destroy_value(BList blist, DestroyFunc destroy_value) {
	if (destroy_value != NULL) {
		blist->destroy_value = destroy_value;
	}
	return destroy_value == NULL ? NULL : destroy_value;
}

void blist_destroy(BList blist) {
	assert(blist != NULL);
	
	BListNode node, next;
	node = blist->first;
	
	//Προσπελαύνουμε την λίστα:
	while (node != BLIST_EOF) {
		//Καταστρέφουμε τα values πρώτα
		if (blist->destroy_value != NULL)
			blist->destroy_value(node->value);
		//Αποθηκεύουμε τον επόμενο κόμβο και καταστρεφουμε τον τρέχον
		next = node->next;
		free(node);
		//και μεταβαίνουμε στον επόμενο κόμβο
		node = next;
	}

	//Αφού καθαρίσουμε το περιεχόμενο της λίστας, πρέπει να καθαρίσουμε και το header της
	free(blist);
}


// Διάσχιση της λίστας /////////////////////////////////////////////

BListNode blist_first(BList blist) {
	assert(blist != NULL);

	return blist->first;
}

BListNode blist_last(BList blist) {
	assert(blist != NULL);

	return blist->last;
}

BListNode blist_next(BList blist, BListNode node) {
	return node != NULL ? node->next : NULL;
}

BListNode blist_previous(BList blist, BListNode node) {
	return node != NULL ? node->prev : NULL;
}

Pointer blist_node_value(BList blist, BListNode node) {
	return node != NULL ? node->value : NULL;
}

BListNode blist_find_node(BList blist, Pointer value, CompareFunc compare) {
	assert(blist != NULL);
	assert(compare != NULL);
	assert(value != NULL);

	BListNode node = blist->first;

	//Ψάχνουμε σε όλη την λίστα για τον πρώτο κόμβο που έχει value ίδιο με αυτό που δίνει ο χρήστης
	while (node != BLIST_EOF) {
		//Ελέγχουμε τον τρέχον κόμβο και αν είναι αυτός που ψάχνουμε τοτε τον επιστρέφουμε
		if(compare(node->value, value) == 0) 
			return node;
		//προχωράμε στον επόμενο κόμβο
		node = node->next;
	}

	//Αν φτάσουμε σε αυτό το σημείο, τότε ο κόμβος που ψάχνουμε δεν υπάρχει. Επιστρέφουμε NULL
	return NULL;
}