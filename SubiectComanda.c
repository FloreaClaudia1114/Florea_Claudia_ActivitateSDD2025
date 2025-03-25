#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct Comanda {
    unsigned int idComanda;
    char* statusComanda;
    char* numeClient;
    unsigned int nrProduse;
    float sumaPlata;
} Comanda;

typedef struct Nod {
    Comanda* info;
    struct Nod* next;
    struct Nod* prev;
} Nod;

Comanda* initializareComanda(unsigned int idComanda, char* statusComanda, char* numeClient, unsigned int nrProduse, float sumaPlata) {
    Comanda* c = (Comanda*)malloc(sizeof(Comanda));
    c->idComanda = idComanda;
    c->statusComanda = (char*)malloc(strlen(statusComanda) + 1);
    strcpy(c->statusComanda, statusComanda);
    c->numeClient = (char*)malloc(strlen(numeClient) + 1);
    strcpy(c->numeClient, numeClient);
    c->nrProduse = nrProduse;
    c->sumaPlata = sumaPlata;
    return c;
}

Nod* initializareNod(Comanda* c) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    nod->info = c;
    nod->next = NULL;
    nod->prev = NULL;
    return nod;
}

Nod* inserareAlternativ(Nod* cap, Nod** coada, Comanda* c, int* alternanta) {
    Nod* nodNou = initializareNod(c);
    if (cap == NULL) {
        cap = nodNou;
        *coada = nodNou;
    }
    else if (*alternanta % 2 == 0) {
        nodNou->next = cap;
        cap->prev = nodNou;
        cap = nodNou;
    }
    else {
        (*coada)->next = nodNou;
        nodNou->prev = *coada;
        *coada = nodNou;
    }
    (*alternanta)++;
    return cap;
}

void afisare(Nod* cap) {
    while (cap != NULL) {
        printf("%u %s %s %u %5.2f \n", cap->info->idComanda, cap->info->statusComanda,
            cap->info->numeClient, cap->info->nrProduse, cap->info->sumaPlata);
        cap = cap->next;
    }
}

void afisareinversa(Nod* coada) {
    while (coada != NULL) {
        printf("%u %s %s %u %5.2f \n", coada->info->idComanda, coada->info->statusComanda,
            coada->info->numeClient, coada->info->nrProduse, coada->info->sumaPlata);
        coada = coada->prev;
    }
}

void dezalocare(Nod* cap) {
    while (cap != NULL) {
        Nod* aux = cap;
        cap = cap->next;
        free(aux->info->statusComanda);
        free(aux->info->numeClient);
        free(aux->info);
        free(aux);
    }
}

float calculMedie(Nod* cap, char* status) {
    int nrComenzi = 0;
    float valoareTotala = 0;
    while (cap != NULL) {
        if (strcmp(cap->info->statusComanda, status) == 0) {
            valoareTotala += cap->info->nrProduse * cap->info->sumaPlata;
            nrComenzi++;
        }
        cap = cap->next;
    }
    return nrComenzi ? valoareTotala / nrComenzi : 0;
}

int main() {
    unsigned int idComanda;
    char statusComanda[30];
    char numeClient[40];
    unsigned char nrProduse;
    float sumaPlata;

    Nod* cap = NULL;
    Nod* coada = NULL;
    int alternanta = 0;

    FILE* f = fopen("fisier.txt", "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului!\n");
        return 1;
    }

    while (fscanf(f, "%u %s %s %hhu %f", &idComanda, statusComanda, numeClient, &nrProduse, &sumaPlata) == 5) {
        Comanda* comanda = initializareComanda(idComanda, statusComanda, numeClient, nrProduse, sumaPlata);
        cap = inserareAlternativ(cap, &coada, comanda, &alternanta);
    }
    fclose(f);

    printf("AFISARE\n");
    afisare(cap);
    printf("-----------------------------\n");

    printf("AFISARE INVERSA\n");
    afisareinversa(coada);
    printf("-------------------------\n");

    printf("CALCUL MEDIE\n");
    float valoareMedie = calculMedie(cap, "Livrata");
    printf("%5.2f\n", valoareMedie);

    dezalocare(cap);
    return 0;
}