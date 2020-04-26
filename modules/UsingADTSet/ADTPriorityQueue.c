//Για αρχή συμπεριλαμβάνουμε τα κατάλληλα header files

#include "ADTPriorityQueue.h"
#include "ADTSet.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h> //Ο μόνος λόγος για να συμπεριλάβουμε την string.h έιναι για να χρησιμοποιήσουμε την memcpy

void destroy_pq_node(Pointer node) {
    free(node);
}

int compare_pq_nodes(Pointer a, Pointer b) {
    return 1; //δεν έχει σημασία η σύγκριση των pq nodes
}

// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
    Set set; // το set στην οποία αποθηκεύουμε τα στοιχεία
    Set pq_node_set; //το set στο οποίο θα αποθηκεύω όλα τα pq_nodes ώστε να μην έχω leaks μετά
};

struct priority_queue_node {
    SetNode node;
};
// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
    assert(compare != NULL);
    PriorityQueue pqueue = malloc(sizeof(*pqueue));
    
    //Η priority queue "είναι" ένα set οπότε το μόνο που έχουμε να κάνουμε είναι
    //να δημιουργήσουμε ένα set και να βάλουμε τα στοιχεία του value_vector αν αυτό είναι γεμάτο
    pqueue->set = set_create(compare, destroy_value);
    
    //δημιουργούμε και το set των pq nodes
    pqueue->pq_node_set = set_create(compare_pq_nodes, destroy_pq_node);

    if(values != NULL) {
        //Για να είμαστε σίγουροι ότι τα στοιχεία του vector 
        //δεν θα διαγραφουν θέτουμε την destroy function σε NULL
        vector_set_destroy_value(values, NULL);

        for (int i = 0; i < vector_size(values); i++) {
            //Προσθέτουμε κάθε τιμή του vector στο set
            set_insert(pqueue->set, vector_get_at(values, i));
        }
    }

    //Επιστρέφουμε την priority queue
    return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
    return set_size(pqueue->set);
}

Pointer pqueue_max(PriorityQueue pqueue) {
    //Εφόσον ξέρουμε ότι το πρώτο στοιχείο του set είναι 
    //το μέγιστο το επιστρέφουμε
    return set_node_value(pqueue->set, set_last(pqueue->set));
}

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value) {
    //Βάζουμε το value στο set
    set_insert(pqueue->set, value);
    PriorityQueueNode pq_node = malloc(sizeof(*pq_node));
    set_insert(pqueue->pq_node_set, pq_node);
    
    pq_node->node = set_find_node(pqueue->set, value);
    
    return pq_node;
}

void pqueue_remove_max(PriorityQueue pqueue) {
    //Απλά κάλεσε την κατάλληλη συνάρτηση του set
    set_remove(pqueue->set, set_node_value(pqueue->set, set_last(pqueue->set)));
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
    // Απλά καλούμε την αντίστοιχη συνάρτηση της λίστας
    return set_set_destroy_value(pqueue->set, destroy_value);
}

void pqueue_destroy(PriorityQueue pqueue) {
    //Καλούμε την set_destroy
    set_destroy(pqueue->set);
    set_destroy(pqueue->pq_node_set);
    //και καταστρέφουμε και την μνήμη που καταλαμβάνει η pqueue
    free(pqueue);
}

Pointer pqueue_node_value(PriorityQueue pqueue, PriorityQueueNode node) {
    //Θυμηθείτε ότι το PriorityQueueNode είναι SetNode
    return set_node_value(pqueue->set, node->node);
}

void pqueue_remove_node(PriorityQueue pqueue, PriorityQueueNode node) {
    set_remove(pqueue->set, set_node_value(pqueue->set, node->node));
}

void pqueue_update_order(PriorityQueue pqueue, PriorityQueueNode node) {
    //H συνάρτηση αυτή θα διαγράφει το node και θα προσθέτει άλλο node με το ίδιο value
    // αφού το node αυτο είναι προβληματικό γιατί δεν είναι ταξινομημένο μετά την αλλαγή
    Pointer item = set_node_remove_specific(pqueue->set, node->node);
    //και τον ξαναπροσθέτουμε
    pqueue_insert(pqueue, item);
}