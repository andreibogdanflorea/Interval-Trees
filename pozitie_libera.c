#include "pozitie_libera.h"


DateIntrare* citeste_fisier_in(char *nume_fisier_intrare) {
    FILE *f = fopen(nume_fisier_intrare, "r");
    DateIntrare* date_intrare = malloc(sizeof(DateIntrare));
    fscanf(f, "%d %d", &date_intrare->numarul_maxim_din_intrari, &date_intrare->numar_intrari);
    date_intrare->intrari = malloc(date_intrare->numar_intrari * sizeof(int));
    for (int i = 0; i < date_intrare->numar_intrari; i++) {
    	fscanf(f, "%d", &date_intrare->intrari[i]);
    }
    fclose(f);
    return date_intrare;
}

int max(int a, int b) {
	return a > b? a: b;
}

void actualizare_marcheaza_pozitie(Nod *nod, int v_actualizare) {
	int valoare_stanga = -1, valoare_dreapta = -1; // valorile predefinite, pentru cazul in care nodul ar fi frunza
	// daca nu e frunza, inseamna ca are 2 copii, si iau maximul dintre aceste valori
	if (nod->copil_stanga != NULL) 
		valoare_stanga = nod->copil_stanga->info;
	if (nod->copil_dreapta != NULL)
		valoare_dreapta = nod->copil_dreapta->info;
	nod->info = max(valoare_stanga, valoare_dreapta);
}

int combina_max(int raspuns_stanga, int raspuns_dreapta) {
	return max(raspuns_stanga, raspuns_dreapta);
}

int* calculeaza_vector_raspuns(DateIntrare *date_intrare) {
    int *lista = calloc(date_intrare->numarul_maxim_din_intrari + 1, sizeof(int));
    ArboreDeIntervale *arbore = construieste_arbore(0, date_intrare->numarul_maxim_din_intrari,
    	-1, actualizare_marcheaza_pozitie, combina_max); // valoarea predefinita de raspuns este -1,
    // deoarece nu influenteaza la maxim conform functiei combina
    seteaza_info_in_arbore_la_valoare_capat_dreapta(arbore);
    // (1)
    // proprietate pe care m-am bazat: fiecare nod indica cea mai din dreapta valoare din intervalul
    // sau asociat la care pot adauga un element
    for (int i = 0; arbore->radacina->info != -1 && i < date_intrare->numar_intrari; i++) {
    	int element = date_intrare->intrari[i];
    	Interval *interval = malloc(sizeof(Interval));
    	interval->capat_stanga = 0; interval->capat_dreapta = element; // caut o pozitie in intervalul [0, element]
    	int pozitie = interogare_interval_in_arbore(arbore, interval); // gasesc pozitia la care trebuie sa adaug elementul curent
    	if (pozitie < 0) // daca toate pozitiile de la 0 la element sunt ocupate, nu am unde sa il adaug
    		continue;
    	lista[pozitie] = element; // altfel, il adaug
    	interval->capat_stanga = pozitie; interval->capat_dreapta = pozitie;
    	actualizare_interval_in_arbore(arbore, interval, 0); // actualizez arborele marcand ca am ocupat pozitia gasita
    	// si pastrand proprietatea (1)
    	free(interval);
    }

    return lista;
}


int* calculeaza_vector_raspuns_trivial(DateIntrare *date_intrare) {
    int *lista = calloc(date_intrare->numarul_maxim_din_intrari + 1, sizeof(int));
    for (int i = 0; i < date_intrare->numar_intrari; i++) {
    	int element = date_intrare->intrari[i];
		for (int j = element; j >= 0; j--) { // ma uit de la pozitia corespunzatoare elementului inspre 0
			if (lista[j] == 0) { // daca pozitia e neocupata
				lista[j] = element; // il adaug
				break;
			}
		}
    }
    return lista;
}