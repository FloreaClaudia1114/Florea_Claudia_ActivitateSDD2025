#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Farmacie {
    unsigned short int idFarmacie;
    char* denumireFarmacie;
    float cifraAfaceriAnuala;
    char* numeFarmacistSef;
    unsigned char nrAngajati;
    char* localitate;
} Farmacie;


typedef struct {
    Farmacie* vector;
    int capacitate;
    int start;
    int final;
    int nrElemente;
} CoadaFarmacie;


Farmacie copieFarmacie(const Farmacie* f) {
    Farmacie copie;
    copie.idFarmacie = f->idFarmacie;
    copie.cifraAfaceriAnuala = f->cifraAfaceriAnuala;
    copie.nrAngajati = f->nrAngajati;

    copie.denumireFarmacie = (char*)malloc(strlen(f->denumireFarmacie) + 1);
    strcpy(copie.denumireFarmacie, f->denumireFarmacie);

    copie.numeFarmacistSef = (char*)malloc(strlen(f->numeFarmacistSef) + 1);
    strcpy(copie.numeFarmacistSef, f->numeFarmacistSef);

    copie.localitate = (char*)malloc(strlen(f->localitate) + 1);
    strcpy(copie.localitate, f->localitate);

    return copie;
}

void elibereazaFarmacie(Farmacie* f) {
    free(f->denumireFarmacie);
    free(f->numeFarmacistSef);
    free(f->localitate);
}


void initCoada(CoadaFarmacie* c, int capacitate) {
    c->vector = (Farmacie*)malloc(sizeof(Farmacie) * capacitate);
    c->capacitate = capacitate;
    c->start = 0;
    c->final = 0;
    c->nrElemente = 0;
}

int esteGoala(CoadaFarmacie* c) {
    return c->nrElemente == 0;
}

int estePlina(CoadaFarmacie* c) {
    return c->nrElemente == c->capacitate;
}


int enqueue(CoadaFarmacie* c, Farmacie f) {
    if (estePlina(c)) {
        printf("Coada este plina, nu se poate adauga!\n");
        return 0;
    }
    c->vector[c->final] = copieFarmacie(&f);
    c->final = (c->final + 1) % c->capacitate;
    c->nrElemente++;
    return 1;
}


int dequeue(CoadaFarmacie* c, Farmacie* f) {
    if (esteGoala(c)) {
        printf("Coada este goala, nu se poate scoate!\n");
        return 0;
    }
    *f = copieFarmacie(&c->vector[c->start]);
    elibereazaFarmacie(&c->vector[c->start]);
    c->start = (c->start + 1) % c->capacitate;
    c->nrElemente--;
    return 1;
}


void distrugeCoada(CoadaFarmacie* c) {
    while (!esteGoala(c)) {
        Farmacie f;
        dequeue(c, &f);
        elibereazaFarmacie(&f);
    }
    free(c->vector);
    c->vector = NULL;
    c->capacitate = 0;
    c->start = 0;
    c->final = 0;
    c->nrElemente = 0;
}


void afiseazaFarmacie(const Farmacie* f) {
    printf("ID: %hu, Denumire: %s, Cifra afaceri: %.2f, Farmacist sef: %s, Nr angajati: %d, Localitate: %s\n",
        f->idFarmacie, f->denumireFarmacie, f->cifraAfaceriAnuala,
        f->numeFarmacistSef, f->nrAngajati, f->localitate);
}


int citesteFarmaciiDinFisier(const char* numeFisier, CoadaFarmacie* coada) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
        return 0;
    }

    unsigned short int id;
    char bufferDenumire[100], bufferNumeFarmacist[100], bufferLocalitate[100];
    float cifra;
    unsigned char nrAng;

    while (fscanf(f, "%hu %99s %f %99s %hhu %99s",
        &id, bufferDenumire, &cifra, bufferNumeFarmacist, &nrAng, bufferLocalitate) == 6) {

        Farmacie fNou;
        fNou.idFarmacie = id;
        fNou.cifraAfaceriAnuala = cifra;
        fNou.nrAngajati = nrAng;

        fNou.denumireFarmacie = (char*)malloc(strlen(bufferDenumire) + 1);
        strcpy(fNou.denumireFarmacie, bufferDenumire);

        fNou.numeFarmacistSef = (char*)malloc(strlen(bufferNumeFarmacist) + 1);
        strcpy(fNou.numeFarmacistSef, bufferNumeFarmacist);

        fNou.localitate = (char*)malloc(strlen(bufferLocalitate) + 1);
        strcpy(fNou.localitate, bufferLocalitate);

        if (!enqueue(coada, fNou)) {
            printf("Coada plina, oprire citire.\n");
            elibereazaFarmacie(&fNou);
            break;
        }
        elibereazaFarmacie(&fNou);
    }

    fclose(f);
    return 1;
}


int main() {
    CoadaFarmacie coada;
    initCoada(&coada, 100);

    if (!citesteFarmaciiDinFisier("farmacii.txt", &coada)) {
        printf("Eroare la citirea fisierului.\n");
        return 1;
    }

    printf("Farmacii citite din fisier (ordine FIFO):\n");
    Farmacie f;
    while (dequeue(&coada, &f)) {
        afiseazaFarmacie(&f);
        elibereazaFarmacie(&f);
    }

    distrugeCoada(&coada);
    return 0;
}
