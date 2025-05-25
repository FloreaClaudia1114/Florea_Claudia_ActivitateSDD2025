#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CursaAeriana {
    char* cod;
    char* destinatie;
    char* dataPlecare;
    char* oraPlecare;
    short int nrMinuteIntarziere;
} CursaAeriana;

typedef struct MaxHeap {
    CursaAeriana** vector;
    int dimensiune;
} MaxHeap;

CursaAeriana* initCursa(char* cod, char* destinatie, char* dataPlecare, char* oraPlecare, short int nrMinuteIntarziere) {
    CursaAeriana* cursaAeriana = (CursaAeriana*)malloc(sizeof(CursaAeriana));

    cursaAeriana->cod = (char*)malloc(strlen(cod) + 1);
    strcpy(cursaAeriana->cod, cod);

    cursaAeriana->destinatie = (char*)malloc(strlen(destinatie) + 1);
    strcpy(cursaAeriana->destinatie, destinatie);

    cursaAeriana->dataPlecare = (char*)malloc(strlen(dataPlecare) + 1);
    strcpy(cursaAeriana->dataPlecare, dataPlecare);

    cursaAeriana->oraPlecare = (char*)malloc(strlen(oraPlecare) + 1);
    strcpy(cursaAeriana->oraPlecare, oraPlecare);

    cursaAeriana->nrMinuteIntarziere = nrMinuteIntarziere;

    return cursaAeriana;
}

// heapify
void filtrareHeap(CursaAeriana** vector, int dimensiune, int index) {
    int max = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < dimensiune && vector[left]->nrMinuteIntarziere > vector[max]->nrMinuteIntarziere) {
        max = left;
    }
    if (right < dimensiune && vector[right]->nrMinuteIntarziere > vector[max]->nrMinuteIntarziere) {
        max = right;
    }

    if (max != index) {
        CursaAeriana* temp = vector[index];
        vector[index] = vector[max];
        vector[max] = temp;

        filtrareHeap(vector, dimensiune, max);
    }
}

void afisareHeap(MaxHeap* maxHeap) {
    for (int i = 0; i < maxHeap->dimensiune; i++) {
        printf("Pozitie %d: \n", i);
        printf("%s %s %s %s %u \n",
            maxHeap->vector[i]->cod,
            maxHeap->vector[i]->destinatie,
            maxHeap->vector[i]->dataPlecare,
            maxHeap->vector[i]->oraPlecare,
            maxHeap->vector[i]->nrMinuteIntarziere);
    }
}

void dezalocare(MaxHeap* maxHeap) {
    if (maxHeap != NULL) {
        for (int i = 0; i < maxHeap->dimensiune; i++) {
            free(maxHeap->vector[i]->cod);
            free(maxHeap->vector[i]->destinatie);
            free(maxHeap->vector[i]->dataPlecare);
            free(maxHeap->vector[i]->oraPlecare);
            free(maxHeap->vector[i]);
        }
        free(maxHeap->vector);
        free(maxHeap);
    }
}

// Numără cursele cu întârziere peste un prag
int nrCurseIntarzierePestePrag(MaxHeap* maxHeap, short int pragMinute) {
    int contor = 0;
    for (int i = 0; i < maxHeap->dimensiune; i++) {
        if (maxHeap->vector[i]->nrMinuteIntarziere > pragMinute) {
            contor++;
        }
    }
    return contor;
}

// Șterge cursele fără întârziere
MaxHeap* stergeCurseDupaIntarziere(MaxHeap* maxHeap) {
    int index = 0;
    for (int i = 0; i < maxHeap->dimensiune; i++) {
        if (maxHeap->vector[i]->nrMinuteIntarziere > 0) {
            maxHeap->vector[index++] = maxHeap->vector[i];
        }
        else {
            // eliberează memoria pentru cursele eliminate
            free(maxHeap->vector[i]->cod);
            free(maxHeap->vector[i]->destinatie);
            free(maxHeap->vector[i]->dataPlecare);
            free(maxHeap->vector[i]->oraPlecare);
            free(maxHeap->vector[i]);
        }
    }

    maxHeap->dimensiune = index;

    // refacem heap-ul
    for (int i = maxHeap->dimensiune / 2 - 1; i >= 0; i--) {
        filtrareHeap(maxHeap->vector, maxHeap->dimensiune, i);
    }

    return maxHeap;
}

// Conversie heap în vector cu dată < dataPlecare
CursaAeriana** conversieHeapInVector(MaxHeap* maxHeap, char* dataPlecare, int* dimensiune) {
    int contor = 0;
    for (int i = 0; i < maxHeap->dimensiune; i++) {
        if (strcmp(maxHeap->vector[i]->dataPlecare, dataPlecare) < 0) {
            contor++;
        }
    }

    if (contor == 0) {
        *dimensiune = 0;
        return NULL;
    }

    CursaAeriana** vectorNou = (CursaAeriana**)malloc(sizeof(CursaAeriana*) * contor);
    int dimensiuneNoua = 0;

    for (int i = 0; i < maxHeap->dimensiune; i++) {
        if (strcmp(maxHeap->vector[i]->dataPlecare, dataPlecare) < 0) {
            vectorNou[dimensiuneNoua++] = maxHeap->vector[i];
        }
    }

    *dimensiune = contor;
    return vectorNou;
}

int main() {
    CursaAeriana* cursa1 = initCursa("RO123", "Paris", "2024-06-20", "20:00", 30);
    CursaAeriana* cursa2 = initCursa("RO124", "Lion", "2025-08-20", "20:00", 30);
    CursaAeriana* cursa3 = initCursa("RO126", "Bali", "2023-09-28", "20:00", 30);
    CursaAeriana* cursa4 = initCursa("RO127", "USA", "2025-01-23", "22:00", 0);   // fără întârziere
    CursaAeriana* cursa5 = initCursa("RO129", "Cluj", "2024-01-26", "20:00", 30);

    MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    maxHeap->dimensiune = 5;
    maxHeap->vector = (CursaAeriana**)malloc(sizeof(CursaAeriana*) * 5);

    maxHeap->vector[0] = cursa1;
    maxHeap->vector[1] = cursa2;
    maxHeap->vector[2] = cursa3;
    maxHeap->vector[3] = cursa4;
    maxHeap->vector[4] = cursa5;

    // construim heap
    for (int i = maxHeap->dimensiune / 2 - 1; i >= 0; i--) {
        filtrareHeap(maxHeap->vector, maxHeap->dimensiune, i);
    }

    printf("=== HEAP INIȚIAL ===\n");
    afisareHeap(maxHeap);

    printf("\nNumar curse cu intarziere > 10 minute: %d\n", nrCurseIntarzierePestePrag(maxHeap, 10));

    maxHeap = stergeCurseDupaIntarziere(maxHeap);
    printf("\n=== DUPA STERGERE CURSE FARA INTARZIERE ===\n");
    afisareHeap(maxHeap);

    int dimVector = 0;
    CursaAeriana** vectorNou = conversieHeapInVector(maxHeap, "2024-12-31", &dimVector);
    printf("\n=== CURSE CU DATA < 2024-12-31 ===\n");
    for (int i = 0; i < dimVector; i++) {
        printf("%s %s %s %s %d\n",
            vectorNou[i]->cod,
            vectorNou[i]->destinatie,
            vectorNou[i]->dataPlecare,
            vectorNou[i]->oraPlecare,
            vectorNou[i]->nrMinuteIntarziere);
    }

    free(vectorNou);  // eliberăm doar containerul, nu elementele (sunt în heap)

    dezalocare(maxHeap);

    return 0;
}
