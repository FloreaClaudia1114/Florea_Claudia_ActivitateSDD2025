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


typedef struct NodStiva {
    Farmacie info;
    struct NodStiva* next;
} NodStiva;

typedef struct {
    NodStiva* varf;
} StivaFarmacie;


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


void initStiva(StivaFarmacie* s) {
    s->varf = NULL;
}

int esteGoala(StivaFarmacie* s) {
    return s->varf == NULL;
}


int push(StivaFarmacie* s, Farmacie f) {
    NodStiva* nodNou = (NodStiva*)malloc(sizeof(NodStiva));
    if (!nodNou) {
        printf("Eroare la alocarea memoriei pentru nod nou.\n");
        return 0;
    }

    nodNou->info = copieFarmacie(&f);
    nodNou->next = s->varf;
    s->varf = nodNou;
    return 1;
}


int pop(StivaFarmacie* s, Farmacie* f) {
    if (esteGoala(s)) {
        printf("Stiva este goala, nu se poate scoate!\n");
        return 0;
    }

    NodStiva* temp = s->varf;
    *f = copieFarmacie(&temp->info);
    s->varf = temp->next;


    elibereazaFarmacie(&temp->info);
    free(temp);

    return 1;
}


void distrugeStiva(StivaFarmacie* s) {
    NodStiva* temp;
    while (s->varf) {
        temp = s->varf;
        s->varf = s->varf->next;
        elibereazaFarmacie(&temp->info);
        free(temp);
    }
}


void afiseazaFarmacie(const Farmacie* f) {
    printf("ID: %hu, Denumire: %s, Cifra afaceri: %.2f, Farmacist sef: %s, Nr angajati: %d, Localitate: %s\n",
        f->idFarmacie, f->denumireFarmacie, f->cifraAfaceriAnuala,
        f->numeFarmacistSef, f->nrAngajati, f->localitate);
}


int citesteFarmaciiDinFisier(const char* numeFisier, StivaFarmacie* stiva) {
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

        if (!push(stiva, fNou)) {
            printf("Stiva plina, oprire citire.\n");
            elibereazaFarmacie(&fNou);
            break;
        }
        elibereazaFarmacie(&fNou);
    }

    fclose(f);
    return 1;
}


int main() {
    StivaFarmacie stiva;
    initStiva(&stiva);

    if (!citesteFarmaciiDinFisier("farmacii.txt", &stiva)) {
        printf("Eroare la citirea fisierului.\n");
        return 1;
    }

    printf("Farmacii citite din fisier:\n");

    StivaFarmacie stivaTemp;
    initStiva(&stivaTemp);

    Farmacie f;
    while (pop(&stiva, &f)) {
        afiseazaFarmacie(&f);
        push(&stivaTemp, f);
        elibereazaFarmacie(&f);
    }


    while (pop(&stivaTemp, &f)) {
        push(&stiva, f);
        elibereazaFarmacie(&f);
    }
    distrugeStiva(&stivaTemp);

    printf("\nScoatem elementele din stiva:\n");
    while (pop(&stiva, &f)) {
        afiseazaFarmacie(&f);
        elibereazaFarmacie(&f);
    }

    distrugeStiva(&stiva);
    return 0;
}
