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
	// ...
};

struct blist_node {
	// ...
};


BList blist_create(DestroyFunc destroy_value) {
	return NULL;
}

int blist_size(BList blist) {
	return 0;
}

void blist_insert(BList blist, BListNode node, Pointer value) {

}

void blist_remove(BList blist, BListNode node) {

}

Pointer blist_find(BList blist, Pointer value, CompareFunc compare) {
	return NULL;
}

DestroyFunc blist_set_destroy_value(BList blist, DestroyFunc destroy_value) {
	return NULL;
}

void blist_destroy(BList blist) {

}


// Διάσχιση της λίστας /////////////////////////////////////////////

BListNode blist_first(BList blist) {
	return NULL;
}

BListNode blist_last(BList blist) {
	return NULL;
}

BListNode blist_next(BList blist, BListNode node) {
	return NULL;
}

BListNode blist_previous(BList blist, BListNode node) {
	return NULL;
}

Pointer blist_node_value(BList blist, BListNode node) {
	return NULL;
}

BListNode blist_find_node(BList blist, Pointer value, CompareFunc compare) {
	return NULL;
}