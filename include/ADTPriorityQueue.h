///////////////////////////////////////////////////////////////////
//
// ADT Priority Queue
//
// Abstract ουρά προτεραιότητας. Σε κάθε remove αφαιρείται το
// μεγαλύτερο στοιχείο (με βάση τη συνάρτηση compare).
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"
#include "ADTVector.h"


// Μία ουρά προτεραιότητας αναπαριστάται από τον τύπο PriorityQueue

typedef struct priority_queue* PriorityQueue;
typedef struct priority_queue_node* PriorityQueueNode;


// Δημιουργεί και επιστρέφει μια νέα ουρά προτεραιότητας, της οποίας τα στοιχεία συγκρίνονται με βάση τη συνάρτηση compare.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται ένα στοιχείο.
// Αν values != NULL, τότε η ουρά αρχικοποιείται με τα στοιχεία του Vector values.

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά pqueue

int pqueue_size(PriorityQueue pqueue);

// Επιστρέφει το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

Pointer pqueue_max(PriorityQueue pqueue);

// Προσθέτει την τιμή value στην ουρά pqueue. Επιστρέφει τον κόμβο που προστέθηκε.
//
// ΠΡΟΣΟΧΗ:
// Όσο το value είναι μέλος της ουράς, οποιαδήποτε μεταβολή στο περιεχόμενό του (στη μνήμη που δείχνει) δεν πρέπει
// να αλλάζει τη σχέση διάταξης (compare) με οποιοδήποτε άλλο στοιχείο, διαφορετικά έχει μη ορισμένη συμπεριφορά.
//
// Για μεταβολές στη διάταξη υπάρχουν 2 επιλογές:
// - αφαίρεση του στοιχείου, μεταβολή και ξανά προσθήκη
// - χρήση της συνάρτησης pqueue_update_order

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value);

// Αφαιρεί την μεγαλύτερη τιμή της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

void pqueue_remove_max(PriorityQueue pqueue);

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η ουρά pqueue.
// Οποιαδήποτε λειτουργία πάνω στη ουρά μετά το destroy είναι μη ορισμένη.

void pqueue_destroy(PriorityQueue pqueue);


//// Νέες συναρτήσεις για την εργασία 2 //////////////////////////////////////////

// Επιστρέφει το περιεχόμενο του κόμβου node

Pointer pqueue_node_value(PriorityQueue set, PriorityQueueNode node);

// Αφαιρεί τον κόμβο node ο οποίος μπορεί να βρίσκεται σε οποιαδήποτε θέση της ουράς
// (μη ορισμένη συμπεριφορά αν δεν ανήκει στην ουρά).

void pqueue_remove_node(PriorityQueue pqueue, PriorityQueueNode node);

// Επιτρέπει την αλλαγή της διάταξης της τιμής του κόμβου node. Η μόνη χρήση που επιτρέπεται είναι η εξής:
// 1. Αλλαγή της διάταξης του κόμβου node (αλλάζοντας τα περιεχόμενα του αντίστοιχου value)
// 2. Κλήση της συνάρτησης pqueue_remove_node για αποκατάσταση της σειράς.
//
// Ανάμεσα στα βήματα 1,2 ΔΕΝ επιτρέπεται:
// - η αλλαγή οποιουδήποτε άλλου κόμβου
// - η κλήση οποιασδήποτε άλλης συνάρτησης pqueue_*

void pqueue_update_order(PriorityQueue pqueue, PriorityQueueNode node);