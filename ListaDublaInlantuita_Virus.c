#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Creare structura Virus
typedef struct Virus {

    unsigned int cod;
    char* nume;
    char* tip;
    int nrTulpini;

}Virus;

// Functia de initializare
Virus initializareVirus(unsigned int cod, const char* nume,
    const char* tip, int nrTulpini) {

    Virus v;
    v.cod = cod;
    v.nume = (char*)malloc(sizeof(char) * strlen(nume) + 1);
    strcpy(v.nume, nume);
    v.tip = (char*)malloc(sizeof(char) * strlen(tip) + 1);
    strcpy(v.tip, tip);
    v.nrTulpini = nrTulpini;

    return v;

}

// Creare structura Nod
typedef struct Nod {

    Virus virus;
    struct Nod* next;
    struct Nod* prev;

}Nod;

// Citire din fisier
Virus citireFisier(FILE* f) {

    char buffer[100];
    Virus v = { 0, NULL, NULL, 0 };

    if (fgets(buffer, sizeof(buffer), f)) {

        char* token = strtok(buffer, ",;\n");
        if (token == NULL) {

            return v;

        }
        v.cod = atoi(token);

        token = strtok(NULL, ",;\n");
        if (token == NULL) {

            return v;

        }
        v.nume = (char*)malloc(sizeof(char) * strlen(token) + 1);
        strcpy(v.nume, token);

        token = strtok(NULL, ",;\n");
        if (token == NULL) {

            return v;

        }
        v.tip = (char*)malloc(sizeof(char) * strlen(token) + 1);
        strcpy(v.tip, token);

        token = strtok(buffer, ",;\n");
        if (token == NULL) {

            return v;

        }
        v.nrTulpini = atoi(token);

    }

    return v;

}

// Functia de afisare a Virusului
void afisareVirus(Virus virus) {

    printf("Cod: %u\n", virus.cod);
    printf("Nume: %s\n", virus.nume);
    printf("Tip: %s\n", virus.tip);
    printf("Nr tulpini: %d\n", virus.nrTulpini);

}

// Creare structura lista dubla
typedef struct ListaDubla {

    Nod* first;
    Nod* last;
    int nrNoduri;

}ListaDubla;

// Functia de afisare a listei de la inceput
void afisareListaInceput(ListaDubla lista) {

    Nod* p = lista.first;
    while (p) {

        afisareVirus(p->virus);
        p = p->next;

    }

}

// Functia de afisare a listei de la sfarsit
void afisareListaSfarsit(ListaDubla lista) {

    Nod* p = lista.last;
    while (p) {

        afisareVirus(p->virus);
        p = p->prev;

    }

}

// Functia de adaugare a unui element la finalul listei
void adaugaLaFinalulListei(ListaDubla* lista, Virus virusNou) {

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->virus = virusNou;
    nou->next = NULL;
    nou->prev = lista->last;

    if (lista->last != NULL) {

        lista->last->next = nou;

    }
    else {

        lista->first = nou;

    }

    lista->last = nou;
    lista->nrNoduri++;

}

// Functia de adaugare a unui element la inceputul listei
void adaugaLaInceputulListei(ListaDubla* lista, Virus virusNou) {

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->virus = virusNou;
    nou->next = lista->first;
    nou->prev = NULL;

    if (lista->first != NULL) {

        lista->first->prev = nou;

    }
    else {

        lista->last = nou;

    }

    lista->first = nou;
    lista->nrNoduri++;

}

// Functia de citire a elementelor din fisier
ListaDubla citireLDDinFisier(const char* numeFisier) {

    FILE* f = fopen(numeFisier, "r");
    ListaDubla lista;
    lista.first = NULL;
    lista.last = NULL;
    lista.nrNoduri = 0;

    while (!feof(f)) {

        adaugaLaFinalulListei(&lista, citireFisier(f));

    }

    fclose(f);
    return lista;

}

// Functia de dezalocare
void dezalocareLista(ListaDubla* lista) {

    Nod* p = lista->first;
    while (p) {

        Nod* aux = p;
        p = p->next;

        if (aux->virus.nume != NULL) {

            free(aux->virus.nume);

        }

        if (aux->virus.tip != NULL) {

            free(aux->virus.tip);

        }

        free(aux);

    }

    lista->first = NULL;
    lista->last = NULL;
    lista->nrNoduri = 0;

}

// Afisare in main
int main() {

    ListaDubla lista = citireLDDinFisier("virus.txt");
    afisareListaInceput(lista);

    printf("\n ===========  Afisare lista de la sfarsit  ============ \n");
    afisareListaSfarsit(lista);

    dezalocareLista(&lista);

    printf("\n ===========  Afisare lista dupa dezalocare  ============ \n");
    afisareListaInceput(lista);

    return 0;

}