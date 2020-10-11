#include <stdio.h>
#include <stdlib.h>
#include "intersectii.h"


#define ASSERT(cond, msg, puncte) if (!(cond)) { failed(msg); } else { passed(msg, puncte); }

void failed(char *s) {
    printf("Testul %s NU a fost trecut!\n", s);
}

void passed(char *s, int puncte) {
    printf("Testul %s a fost trecut!\t\t\t %d puncte\n", s, puncte);
}

void test_entry(char *nume_fisier, int raspuns) {
    ListaIntervale2D *lista_intervale2D = citeste_intrare(nume_fisier);
    int intersectii = calculeaza_numar_intersectii(lista_intervale2D);
    ASSERT(intersectii == raspuns, "1 segment arbore", 15);
    int v2 = calculeaza_numar_intersectii_trivial(lista_intervale2D);
    ASSERT(v2 == raspuns, "2 segment trivial", 5);
}

int main() {
    test_entry("tests/intersectie1.in", 4);
    test_entry("tests/intersectie2.in", 5);
    return 0;
}