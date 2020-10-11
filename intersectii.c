#include "intersectii.h"


// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}


ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {
    FILE *fisier = fopen(nume_fisier_intrare, "r");
    ListaIntervale2D *lista = malloc(sizeof(ListaIntervale2D));
    fscanf(fisier, "%d", &lista->dimensiune);
    lista->intervale = malloc(lista->dimensiune * sizeof(Interval2D*));
    for (int i = 0; i < lista->dimensiune; i++) {
        Punct2D *punct_stanga = malloc(sizeof(Punct2D));
        Punct2D *punct_dreapta = malloc(sizeof(Punct2D));
        fscanf(fisier, "%d %d %d %d", &punct_stanga->x, &punct_stanga->y, 
            &punct_dreapta->x, &punct_dreapta->y); // citesc punctele
        // atribui punctele unui interval
        Interval2D *interval = malloc(sizeof(Interval2D));
        interval->punct_stanga = punct_stanga;
        interval->punct_dreapta = punct_dreapta;
        lista->intervale[i] = interval; // adaug intervalul
    }
    fclose(fisier);
    return lista;
}

void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}

int se_intersecteaza(Interval2D interval_orizontal, Interval2D interval_vertical) {
    if (interval_orizontal.punct_stanga->x <= interval_vertical.punct_stanga->x && // daca segmentul vertical este pe Ox intre
        interval_vertical.punct_stanga->x <= interval_orizontal.punct_dreapta->x && // capetele segmentului orizontal
        interval_orizontal.punct_stanga->y >= interval_vertical.punct_stanga->y && // daca segmentul orizontal este pe Oy intre
        interval_orizontal.punct_stanga->y <= interval_vertical.punct_dreapta->y) // capetele segmentului vertical
        return 1;
    return 0;
}

int e_interval_orizontal(Interval2D interval) {
    return interval.punct_stanga->y == interval.punct_dreapta->y;
}

int e_interval_vertical(Interval2D interval) {
    return interval.punct_stanga->x == interval.punct_dreapta->x;
}

int max(int a, int b) {
    return a > b? a: b;
}

int y_max(ListaIntervale2D *lista) { // calculeaza cea mai mare y din lista (y-ul celui mai de sus punct)
    int max_y = lista->intervale[0]->punct_stanga->y;
    for (int i = 0; i < lista->dimensiune; i++) {
        max_y = max(max_y, lista->intervale[i]->punct_dreapta->y);
    }
    return max_y;
}

int comp(const void *punct1, const void *punct2) { // folosita la sortarea listei de puncte
    Punct2D punct1_2D = ((PunctCuInterval*)punct1)->punct;
    Punct2D punct2_2D = ((PunctCuInterval*)punct2)->punct;
    return punct1_2D.x - punct2_2D.x; // sortez in ordine crescatoare dupa coordonata x a punctelor
}

void sortare_puncte(PunctCuInterval *lista, int dimensiune) {
    qsort(lista, dimensiune, sizeof(PunctCuInterval), comp);
}

int calculeaza_numar_intersectii(ListaIntervale2D *lista) {
    int intersectii = 0;
    ArboreDeIntervale* arbore = construieste_arbore(0, y_max(lista), 0, actualizare_cu_delta, suma_raspunsurilor);
    // creez o lista de puncte cu intervalul asociat, avand ca dimensiune 2*dimensiunea listei cu segmente,
    // pentru ca fiecare segment are 2 puncte
    PunctCuInterval *lista_puncte = malloc(2 * lista->dimensiune * sizeof(PunctCuInterval));
    for (int i = 0; i < lista->dimensiune; i++) {
        // fac atribuirile necesare
        lista_puncte[2 * i].punct = *lista->intervale[i]->punct_stanga;
        lista_puncte[2 * i + 1].punct = *lista->intervale[i]->punct_dreapta;
        lista_puncte[2 * i].interval = lista->intervale[i];
        lista_puncte[2 * i + 1].interval = lista->intervale[i]; 
    }
    sortare_puncte(lista_puncte, 2 * lista->dimensiune); // sortez crescator punctele dupa coordonata x

    for (int i = 0; i < 2 * lista->dimensiune; i++) {
        PunctCuInterval punctSeg = lista_puncte[i];
        if (e_interval_orizontal(*punctSeg.interval)) { // daca punctul face parte dintr-un segment orizontal
            Interval *interval = malloc(sizeof(Interval));
            interval->capat_stanga = punctSeg.punct.y; interval->capat_dreapta = punctSeg.punct.y;
            // daca punctul e capatul stanga al intervalului din care face parte, cresc cu 1
            // valoarea de la intervalul [y, y], y al punctului, din arborele de intervale
            // altfel, inseamna ca e capatul dreapta al intervalului orizontal, ceea ce
            // inseamna ca scad 1 din intervalul [y, y]
            if (punctSeg.punct.x == punctSeg.interval->punct_stanga->x) {
                actualizare_interval_in_arbore(arbore, interval, 1);
            } else {
                actualizare_interval_in_arbore(arbore, interval, -1);
            }
            free(interval);
        } else if (punctSeg.punct.y == punctSeg.interval->punct_stanga->y){ // altfel face parte dintr-un segment vertical
            // intru aici doar pentru unul dintre capetele segmentului vertical, ca sa nu adaug de doua ori aceleasi intersectii
            Interval *interval = malloc(sizeof(Interval));
            interval->capat_stanga = punctSeg.interval->punct_stanga->y; 
            interval->capat_dreapta = punctSeg.interval->punct_dreapta->y;
            intersectii += interogare_interval_in_arbore(arbore, interval);
            free(interval);
        }
    }
    free(lista_puncte);
    return intersectii;
}


int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
    int intersectii = 0;
    for (int i = 0; i < lista->dimensiune - 1; i++) {
        for (int j = i + 1; j < lista->dimensiune; j++) {
            // presupun ca primul e interval orizontal si al doilea e vertical
            Interval2D interval_orizontal = *(lista->intervale[i]), interval_vertical = *(lista->intervale[j]);
            // daca e invers, le interschimb
            if (e_interval_orizontal(interval_vertical) && e_interval_vertical(interval_orizontal)) {
                Interval2D temp = interval_orizontal;
                interval_orizontal = interval_vertical;
                interval_vertical = temp;
            }
            if (e_interval_orizontal(interval_orizontal) && e_interval_vertical(interval_vertical) &&
                se_intersecteaza(interval_orizontal, interval_vertical)) {
                intersectii++;
            }
            // daca sunt de acelasi tip, nu se intampla nimic
        }
    }
    return intersectii;
}
