#include <stdio.h>
#include <stdlib.h>
#include "arbori_intervale.h"


#define ASSERT(cond, msg, puncte) if (!(cond)) { failed(msg); } else { passed(msg, puncte); }

void failed(char *s) {
    printf("Testul %s NU a fost trecut!\n", s);
}

void passed(char *s, int puncte) {
    printf("Testul %s a fost trecut!\t\t\t %d puncte\n", s, puncte);
}


void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}


Interval* construct_interval(int st, int dr) {
    Interval *interval = malloc(sizeof(Interval));
    if (interval == NULL) {
        exit(1);
    }
    interval->capat_stanga = st;
    interval->capat_dreapta = dr;
    return interval;
}


void test_operatie_interval() {
    ArboreDeIntervale *arbore = construieste_arbore(0, 11, 0, actualizare_cu_delta, suma_raspunsurilor);
    Interval *i1 = construct_interval(2, 2);
    Interval *i2 = construct_interval(6, 6);
    Interval *i3 = construct_interval(9, 9);

    actualizare_interval_in_arbore(arbore, i1, 1);
    actualizare_interval_in_arbore(arbore, i3, 1);
    actualizare_interval_in_arbore(arbore, i2, 1);

    Nod *radacina = arbore->radacina;
    ASSERT(radacina->info == 3, "1 actualizare arbore", 3);
    ASSERT(radacina->copil_stanga->info == 1, "2 actualizare arbore", 3);
    ASSERT(radacina->copil_dreapta->info == 2, "3 actualizare arbore", 3);
    ASSERT(radacina->copil_dreapta->copil_stanga->copil_stanga->copil_stanga->info == 1, "4 actualizare arbore", 3);
    Interval *i4 = construct_interval(0, 8);
    ASSERT(interogare_interval_in_arbore(arbore, i4) == 2, "5 interogare arbore", 3);

    Interval *i5 = construct_interval(4, 5);
    actualizare_interval_in_arbore(arbore, i5, 1);

    ASSERT(radacina->copil_stanga->copil_dreapta->copil_stanga->copil_dreapta->info == 1, "6 actualizare arbore", 3);
    ASSERT(radacina->copil_stanga->copil_dreapta->copil_dreapta->info == 1, "7 actualizare arbore", 3);
    ASSERT(interogare_interval_in_arbore(arbore, i4) == 3, "8 interogare arbore", 5);
}


void test_creare_arbore() {
    ArboreDeIntervale *arbore = construieste_arbore(0, 11, 0, actualizare_cu_delta, suma_raspunsurilor);
    Nod *radacina = arbore->radacina;
    ASSERT(radacina->interval->capat_stanga == 0, "1 creare arbore", 3);
    ASSERT(radacina->interval->capat_dreapta == 11, "2 creare arbore", 3);
    ASSERT(radacina->copil_stanga->interval->capat_stanga == 0, "3 creare arbore", 3);
    ASSERT(radacina->copil_stanga->interval->capat_dreapta == 5, "4 creare arbore", 3);
    ASSERT(radacina->copil_dreapta->interval->capat_stanga == 6, "5 creare arbore", 3);
    ASSERT(radacina->copil_dreapta->interval->capat_dreapta == 11, "6 creare arbore", 3);
    Interval *interval_frunza_stanga = radacina->copil_stanga->copil_stanga->copil_stanga->copil_stanga->interval;
    ASSERT(interval_frunza_stanga->capat_stanga == interval_frunza_stanga->capat_dreapta, "7 creare arbore", 3);
    Interval *interval_frunza_dreapta = radacina->copil_dreapta->copil_stanga->copil_dreapta->interval;
    ASSERT(interval_frunza_dreapta->capat_stanga == interval_frunza_dreapta->capat_dreapta, "8 creare arbore", 3);
}


int main() {
    test_creare_arbore();
    test_operatie_interval();
}