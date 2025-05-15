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

void afisarePreordine(NodABC* radacina) {
    if (radacina) {
        printf("%u %s %u %u %.2f\n", radacina->info->id, radacina->info->locatie, radacina->info->capacitate, radacina->info->nivel, radacina->info->temperatura);
        afisarePreordine(radacina->left);
        afisarePreordine(radacina->right);
    }
}

int numaraNoduri(NodABC* radacina) {
    if (radacina == NULL) return 0;
    return 1 + numaraNoduri(radacina->left) + numaraNoduri(radacina->right);
}

int inaltimeArbore(NodABC* radacina) {
    if (radacina == NULL) return 0;
    int stanga = inaltimeArbore(radacina->left);
    int dreapta = inaltimeArbore(radacina->right);
    return 1 + (stanga > dreapta ? stanga : dreapta);
}

NodABC* subarboreMaiInalt(NodABC* radacina) {
    if (radacina == NULL) return NULL;

    int inaltimeStanga = inaltimeArbore(radacina->left);
    int inaltimeDreapta = inaltimeArbore(radacina->right);

    return (inaltimeStanga >= inaltimeDreapta) ? radacina->left : radacina->right;
}

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

    while (fscanf(file, "%u %s %u %u %f", &id, locatie, &capacitate, &nivel, &temperatura) == 5) {
        Depozit* dep = initializareDepozit(id, locatie, capacitate, nivel, temperatura);
        radacina = inserareArbore(radacina, dep);
    }

    fclose(file);

    printf("Afisare arbore in preordine:\n");
    afisarePreordine(radacina);

    printf("\nNumarul total de noduri din arbore: %d\n", numaraNoduri(radacina));

    printf("\nInaltimea arborelui: %d\n", inaltimeArbore(radacina));

    NodABC* subarboreInalt = subarboreMaiInalt(radacina);
    if (subarboreInalt) printf("Subarborele cu inaltimea mai mare incepe cu: %u\n", subarboreInalt->info->id);

    dezalocareArbore(radacina);
    return 0;
}
