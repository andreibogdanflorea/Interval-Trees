#include "pozitie_libera.h"

#define ASSERT(cond, msg, puncte) if (!(cond)) { failed(msg); } else { passed(msg, puncte); }

void failed(char *s) {
    printf("Testul %s NU a fost trecut!\n", s);
}

void passed(char *s, int puncte) {
    printf("Testul %s a fost trecut!\t\t\t %d puncte\n", s, puncte);
}


void test_entry(char *nume_fisier, int *raspuns) {
    DateIntrare *date_intrare = citeste_fisier_in(nume_fisier);
    
    int *raspuns_trivial = calculeaza_vector_raspuns_trivial(date_intrare);
    int *raspuns_arbore = calculeaza_vector_raspuns(date_intrare);
    int flag_t = 1;
    int flag_a = 1;
    int i = 0;
    for (i = 0; i < date_intrare->numarul_maxim_din_intrari + 1; i++) {
        if (raspuns_trivial[i] != raspuns[i]) {
            flag_t = 0;
        }
        if (raspuns_arbore[i] != raspuns[i]) {
            flag_a = 0;
        }
    }
    ASSERT(flag_a == 1, "arbore", 5);
    ASSERT(flag_t == 1, "trivial", 2);
}


int main() {
    int sol1[] = {1, 1, 2, 3, 4, 5, 6, 7};
    int sol2[] = {2, 1, 3, 3, 6, 7, 7, 7};
    int sol3[] = {1, 1, 2, 6, 4, 5, 6};
    printf("Primul test:\n");
    test_entry("tests/pozitie1.in", sol1);
    printf("Al doilea test:\n");    
    test_entry("tests/pozitie2.in", sol2);
    printf("Al treilea test:\n");
    test_entry("tests/pozitie3.in", sol3);
    printf("\n** Punctajul pentru testele triviale va fi trunchiat la 5 puncte.\n");
    return 0;
}