#include "arbori_intervale.h"


Nod *creare_nod(int capat_stanga, int capat_dreapta, int *dimensiune) {
    Nod *nod = malloc(sizeof(Nod));
    nod->interval = malloc(sizeof(Interval));
    nod->interval->capat_stanga = capat_stanga;
    nod->interval->capat_dreapta = capat_dreapta;
    nod->info = 0; // initial pun 0 in toate nodurile
    (*dimensiune)++; // pointer spre dimensiunea arborelui
    if (capat_stanga < capat_dreapta) {
        int mijloc = (capat_stanga + capat_dreapta) / 2;
        nod->copil_stanga = creare_nod(capat_stanga, mijloc, dimensiune);
        nod->copil_dreapta = creare_nod(mijloc + 1, capat_dreapta, dimensiune);
    }
    return nod;
}

ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga, int raspuns_dreapta)) {
    ArboreDeIntervale *arb = malloc(sizeof(ArboreDeIntervale));
    arb->valoare_predifinita_raspuns_copil = valoare_predifinita_raspuns_copil;
    arb->f_actualizare = f_actualizare;
    arb->f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;
    // creeaza un nod cu capatul stanga si dreapta respectiv, si actualizeaza dimensiunea arborelui
    arb->radacina = creare_nod(capat_stanga, capat_dreapta, &arb->dimensiune); 
    return arb;
}


void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {
    if (interval->capat_stanga <= nod->interval->capat_stanga &&
        nod->interval->capat_dreapta <= interval->capat_dreapta) {
        arbore->f_actualizare(nod, v_actualizare);
    } else {
        int mijloc = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;
        if (interval->capat_stanga <= mijloc) {
            actualizare_interval_pentru_nod(arbore, nod->copil_stanga, interval, v_actualizare);
        }
        if (mijloc < interval->capat_dreapta) {
            actualizare_interval_pentru_nod(arbore, nod->copil_dreapta, interval, v_actualizare);
        }
        arbore->f_actualizare(nod, v_actualizare);
    }
}


void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval, v_actualizare);
}


int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod, Interval *interval) {
    if (interval->capat_stanga <= nod->interval->capat_stanga &&
        nod->interval->capat_dreapta <= interval->capat_dreapta) {
        return nod->info;
    } else {
        int mijloc = (nod->interval->capat_stanga + nod->interval->capat_dreapta) / 2;
        int val_stanga = arbore->valoare_predifinita_raspuns_copil;
        int val_dreapta = arbore->valoare_predifinita_raspuns_copil;
        if (interval->capat_stanga <= mijloc) {
            val_stanga = interogare_interval_pentru_nod(arbore, nod->copil_stanga, interval);
        }
        if (mijloc < interval->capat_dreapta) {
            val_dreapta = interogare_interval_pentru_nod(arbore, nod->copil_dreapta, interval);
        }
        return arbore->f_combinare_raspunsuri_copii(val_stanga, val_dreapta);
    }
}


int interogare_interval_in_arbore(ArboreDeIntervale *arbore, Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    if (nod == NULL)
        return; // evident nu am ce sa pun intr-un nod null
    nod->info = nod->interval->capat_dreapta; // pun valoarea din capat_dreapta a intervalului asociat
    seteaza_info_in_nod_la_valoare_capat_dreapta(nod->copil_stanga); // fac acelasi lucru pe subarborele stang
    seteaza_info_in_nod_la_valoare_capat_dreapta(nod->copil_dreapta); // si pe subarborele drept
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}
