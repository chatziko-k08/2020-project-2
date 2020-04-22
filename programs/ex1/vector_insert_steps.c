#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ADTVector.h"

void destroy_int(Pointer value) {
    free(value);
}

void real(void) {
    FILE *fout = fopen("real.csv", "w");
    srand((unsigned int) time(NULL));
    Vector v;
    //Φτιάχνουμε ένα vector για αν εισάγουμε στοιχεία
    v = vector_create(10, destroy_int);

    //Βάλε 10000 στοιχεία στο v και εκτύπωσε τα steps για κάθε ένα από αυτά
    for (int n = 1; n <= 1000; n++) {
        int *p = malloc(sizeof(*p));
        *p = rand()%10000;
        vector_insert_last(v, p);
        fprintf(fout,"%d,<steps for n=%d>\n", vector_steps(v), n);
    }

    vector_destroy(v);
}

void amortized(void) {
    FILE *fout = fopen("amortized.csv", "w");
    //The amortized complexity is estimated by adding all the steps and then dividing them by the number of the steps
    srand((unsigned int)time(NULL));
    Vector v;
    //Φτιάχνουμε ένα vector για αν εισάγουμε στοιχεία
    v = vector_create(10, destroy_int);
    int sum_steps = 0;
    int n;
    //Βάλε 10000 στοιχεία στο v και εκτύπωσε τα steps για κάθε ένα από αυτά
    for (n = 1; n <= 10; n++)
    {
        int *p = malloc(sizeof(*p));
        *p = rand() % 10000;
        vector_insert_last(v, p);
        sum_steps += vector_steps(v);
        fprintf(fout, "%.2f,<steps for n=%d>\n", ((float)sum_steps) / ((float)n), n);
    }
    vector_destroy(v);
}

void error_message(void) {
    printf("Error while trying use the program. Usage:\n\n\t~$ vector_insert_steps type = [real | amortized]\n ----------\n\n");
    exit(-1);
}
int main(int argc, char** argv) {
    if (argc != 4 || strcmp("type", argv[1]) != 0) 
        error_message();


    //Έλεγχοι για το format της κλήσης του προγράμματος και κλήση των κατάλληλων συναρτήσεων
    if (strcmp("=", argv[2]) == 0 || strcmp(":", argv[2]) == 0) {
        if (strcmp("real", argv[3]) == 0)
            real();
        else if (strcmp("amortized", argv[3]) == 0)
            amortized();
        else
            error_message();
    }
    else
        error_message();
    return 0;
}