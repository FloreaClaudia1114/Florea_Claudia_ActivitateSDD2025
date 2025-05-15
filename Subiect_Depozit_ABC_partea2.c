#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Depozit {
    unsigned int id;
    char* locatie;
    unsigned int capacitate;
    unsigned int nivel;
    float temperatura;
} Depozit;

typedef struct NodABC {
    Depozit* info;
    struct NodABC* left;
    struct NodABC* right;
} NodABC;

Depozit* initializareDepozit(unsigned int id, char* locatie, unsigned int capacitate, unsigned int nivel, float temperatura) {
    Depozit* dep = (Depozit*)malloc(sizeof(Depozit));
    dep->id = id;
    dep->locatie = (char*)malloc(strlen(locatie) + 1);
    strcpy(dep->locatie, locatie);
    dep->capacitate = capacitate;
    dep->nivel = nivel;
    dep->temperatura = temperatura;

    return dep;
}

NodABC* initializareNod(Depozit* dep) {
    NodABC* nodNou = (NodABC*)malloc(sizeof(NodABC));
    nodNou->info = dep;
    nodNou->left = NULL;
    nodNou->right = NULL;
    return nodNou;
}

NodABC* inserareArbore(NodABC* radacina, Depozit* dep) {
    if (radacina == NULL) {
        return initializareNod(dep);
    }

    if (dep->id < radacina->info->id) {
        radacina->left = inserareArbore(radacina->left, dep);
    }
    else if (dep->id > radacina->info->id) {
        radacina->right = inserareArbore(radacina->right, dep);
    }

    return radacina;
}

// Functie de afisare preordine
void afisarePreordine(NodABC* radacina) {
    if (radacina) {
        printf("ID: %u, Locatie: %s, Capacitate: %u, Nivel: %u, Temperatura: %.2f\n",
            radacina->info->id, radacina->info->locatie, radacina->info->capacitate,
            radacina->info->nivel, radacina->info->temperatura);
        afisarePreordine(radacina->left);
        afisarePreordine(radacina->right);
    }
}

// Functia 2
int numaraNoduriNivel(NodABC* radacina, unsigned int nivelCautat, Depozit** vector, int* index) {
    if (radacina == NULL) return 0;

    if (radacina->info->nivel == nivelCautat) {
        vector[*index] = (radacina->info);
        (*index)++;
    }

    numaraNoduriNivel(radacina->left, nivelCautat, vector, index);
    numaraNoduriNivel(radacina->right, nivelCautat, vector, index);

    return *index;
}

// Functia 3
int drumRadacina(NodABC* radacina, unsigned int id, unsigned int* vector, int index) {
    if (radacina == NULL) return 0;

    vector[index] = radacina->info->id;

    if (radacina->info->id == id) return index + 1;

    if (id < radacina->info->id) return drumRadacina(radacina->left, id, vector, index + 1);

    return drumRadacina(radacina->right, id, vector, index + 1);
}

// Functia 4
int nivelMaximNoduri(NodABC* radacina) {
    if (radacina == NULL) return 0;

    int stanga = nivelMaximNoduri(radacina->left);
    int dreapta = nivelMaximNoduri(radacina->right);

    return 1 + (stanga > dreapta ? stanga : dreapta);
}

// Functia 5
NodABC* stergeFrunze(NodABC* radacina) {
    if (radacina == NULL) return NULL;

    if (radacina->left == NULL && radacina->right == NULL) {
        free(radacina->info->locatie);
        free(radacina->info);
        free(radacina);
        return NULL;
    }

    radacina->left = stergeFrunze(radacina->left);
    radacina->right = stergeFrunze(radacina->right);

    return radacina;
}

// Functia 6 - Dezalocare totala
void dezalocareArbore(NodABC* radacina) {
    if (radacina) {
        dezalocareArbore(radacina->left);
        dezalocareArbore(radacina->right);
        free(radacina->info->locatie);
        free(radacina->info);
        free(radacina);
    }
}

int main() {
    FILE* file = fopen("depozit.txt", "r");
    if (!file) {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }

    NodABC* radacina = NULL;
    unsigned int id, capacitate, nivel;
    char locatie[100];
    float temperatura;

    // Citim datele si construim arborele
    while (fscanf(file, "%u %s %u %u %f", &id, locatie, &capacitate, &nivel, &temperatura) == 5) {
        Depozit* dep = initializareDepozit(id, locatie, capacitate, nivel, temperatura);
        radacina = inserareArbore(radacina, dep);
    }
    fclose(file);

    printf("Afisare arbore in preordine:\n");
    afisarePreordine(radacina);

    // Folosim functia de numarare noduri la un nivel dat
    unsigned int nivelCautat = 2;
    Depozit* vectorDepozite[100];
    int index = 0;
    int nrGasite = numaraNoduriNivel(radacina, nivelCautat, vectorDepozite, &index);
    printf("\nNoduri cu nivelul %u gasite: %d\n", nivelCautat, nrGasite);
    for (int i = 0; i < nrGasite; i++) {
        printf("ID: %u, Locatie: %s\n", vectorDepozite[i]->id, vectorDepozite[i]->locatie);
    }

    // Folosim functia drumRadacina pentru a afisa drumul pana la un id cautat
    unsigned int idCautat = 3;
    unsigned int drum[100];
    int lungimeDrum = drumRadacina(radacina, idCautat, drum, 0);
    if (lungimeDrum > 0) {
        printf("\nDrumul de la radacina pana la nodul cu ID %u este: ", idCautat);
        for (int i = 0; i < lungimeDrum; i++) {
            printf("%u ", drum[i]);
        }
        printf("\n");
    }
    else {
        printf("\nNodul cu ID %u nu a fost gasit in arbore.\n", idCautat);
    }

    // Folosim functia nivelMaximNoduri
    int nivelMaxim = nivelMaximNoduri(radacina);
    printf("\nInaltimea arborelui este: %d\n", nivelMaxim);

    // Stergem frunzele arborelui
    radacina = stergeFrunze(radacina);
    printf("\nArbore dupa stergerea frunzelor (afisare preordine):\n");
    afisarePreordine(radacina);

    // Dezalocare finala
    dezalocareArbore(radacina);
    return 0;
}