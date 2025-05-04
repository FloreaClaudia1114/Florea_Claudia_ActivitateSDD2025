#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Farmacie {
    int cod;
    char* denumire;
    int nrsucursale;
    float pretmed;
} Farmacie;


void scrieInFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    if (!f) return;

    Farmacie farmacii[10] = {
        {1, "Sensiblu", 5, 12.5},
        {2, "Catena", 8, 10.0},
        {3, "HelpNet", 3, 15.2},
        {4, "Dona", 5, 9.5},
        {5, "Tei", 1, 8.9},
        {6, "Reteta", 3, 11.0},
        {7, "FarmaPlus", 8, 13.0},
        {8, "MediFarm", 2, 14.8},
        {9, "EcoFarm", 1, 9.0},
        {10, "Vita", 5, 12.2}
    };

    for (int i = 0; i < 10; i++) {
        fprintf(f, "%d %s %d %.2f\n", farmacii[i].cod, farmacii[i].denumire,
            farmacii[i].nrsucursale, farmacii[i].pretmed);
    }

    fclose(f);
}

// Citirea din fișier într-un vector
Farmacie* citesteFarmacii(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    Farmacie* vector = NULL;
    char buffer[100];
    *dim = 0;

    while (1) {
        Farmacie temp;
        if (fscanf(f, "%d %s %d %f", &temp.cod, buffer, &temp.nrsucursale, &temp.pretmed) != 4)
            break;

        temp.denumire = (char*)malloc(strlen(buffer) + 1);
        strcpy(temp.denumire, buffer);

        vector = (Farmacie*)realloc(vector, (*dim + 1) * sizeof(Farmacie));
        vector[*dim] = temp;
        (*dim)++;
    }

    fclose(f);
    return vector;
}

// Creează matrice în funcție de nrsucursale
Farmacie** creeazaMatrice(Farmacie* vector, int dim, int* nrLinii, int** dimLinii) {
    int maxSucursale = 0;
    for (int i = 0; i < dim; i++)
        if (vector[i].nrsucursale > maxSucursale)
            maxSucursale = vector[i].nrsucursale;

    *nrLinii = maxSucursale;
    *dimLinii = (int*)calloc(*nrLinii, sizeof(int));
    Farmacie** matrice = (Farmacie**)malloc((*nrLinii) * sizeof(Farmacie*));

    for (int i = 0; i < *nrLinii; i++)
        matrice[i] = NULL;

    for (int i = 0; i < dim; i++) {
        int linie = vector[i].nrsucursale - 1;
        int poz = (*dimLinii)[linie];

        matrice[linie] = (Farmacie*)realloc(matrice[linie], (poz + 1) * sizeof(Farmacie));
        matrice[linie][poz] = vector[i];

        matrice[linie][poz].denumire = (char*)malloc(strlen(vector[i].denumire) + 1);
        strcpy(matrice[linie][poz].denumire, vector[i].denumire);

        (*dimLinii)[linie]++;
    }

    return matrice;
}

// Sortează liniile după numărul de elemente
void sorteazaLinii(Farmacie*** matrice, int* dimLinii, int nrLinii) {
    for (int i = 0; i < nrLinii - 1; i++) {
        for (int j = i + 1; j < nrLinii; j++) {
            if (dimLinii[i] > dimLinii[j]) {
                int aux = dimLinii[i];
                dimLinii[i] = dimLinii[j];
                dimLinii[j] = aux;

                Farmacie* temp = (*matrice)[i];
                (*matrice)[i] = (*matrice)[j];
                (*matrice)[j] = temp;
            }
        }
    }
}

// Afișează matricea
void afiseazaMatrice(Farmacie** matrice, int* dimLinii, int nrLinii) {
    for (int i = 0; i < nrLinii; i++) {
        printf("Linia %d (%d elemente):\n", i + 1, dimLinii[i]);
        for (int j = 0; j < dimLinii[i]; j++) {
            printf("  Cod: %d, Denumire: %s, Sucursale: %d, Pret: %.2f\n",
                matrice[i][j].cod, matrice[i][j].denumire,
                matrice[i][j].nrsucursale, matrice[i][j].pretmed);
        }
        printf("\n");
    }
}

int main() {
    const char* numeFisier = "farmacii.txt";
    scrieInFisier(numeFisier);

    int dim = 0;
    Farmacie* vector = citesteFarmacii(numeFisier, &dim);

    int nrLinii = 0;
    int* dimLinii = NULL;
    Farmacie** matrice = creeazaMatrice(vector, dim, &nrLinii, &dimLinii);

    sorteazaLinii(&matrice, dimLinii, nrLinii);
    afiseazaMatrice(matrice, dimLinii, nrLinii);

    for (int i = 0; i < nrLinii; i++) {
        for (int j = 0; j < dimLinii[i]; j++) {
            free(matrice[i][j].denumire);
        }
        free(matrice[i]);
    }
    free(matrice);
    free(dimLinii);

    for (int i = 0; i < dim; i++) {
        free(vector[i].denumire);
    }
    free(vector);

    return 0;
}
