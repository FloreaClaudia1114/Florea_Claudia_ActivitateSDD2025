#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Serviciu {
    unsigned int idserviciu;
    char* tipserviciu;
    char* nume;
    float sumaplata;
    unsigned int timpestimat;
} Serviciu;

typedef struct Nod {
    Serviciu* info;
    struct Nod* next;
} Nod;

typedef struct HashT {
    Nod** vector;
    int dimensiune;
} HashT;

Serviciu* initializareServiciu(unsigned int idserviciu, char* tipserviciu, char* nume, float sumaplata, unsigned int timpestimat) {
    Serviciu* serviciu = (Serviciu*)malloc(sizeof(Serviciu));
    serviciu->idserviciu = idserviciu;
    serviciu->tipserviciu = (char*)malloc(strlen(tipserviciu) + 1);
    strcpy(serviciu->tipserviciu, tipserviciu);
    serviciu->nume = (char*)malloc(strlen(nume) + 1);
    strcpy(serviciu->nume, nume);
    serviciu->sumaplata = sumaplata;
    serviciu->timpestimat = timpestimat;
    return serviciu;
}

Nod* initializareNod(Serviciu* serviciu) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    nod->info = serviciu;
    nod->next = NULL;
    return nod;
}

HashT* initializareHashT(int dimensiune) {
    HashT* hasht = (HashT*)malloc(sizeof(HashT));
    hasht->dimensiune = dimensiune;
    hasht->vector = (Nod**)malloc(hasht->dimensiune * sizeof(Nod*));
    for (int i = 0; i < hasht->dimensiune; i++) {
        hasht->vector[i] = NULL;
    }
    return hasht;
}

int functieHash(HashT* hasht, char* numeclient) {
    return (numeclient[0] - 'A') % hasht->dimensiune;
}

HashT* inserareinHashT(HashT* hasht, Serviciu* serviciu) {
    if (hasht->vector != NULL) {
        int pozitie = functieHash(hasht, serviciu->nume);
        Nod* nodnou = initializareNod(serviciu);
        if (hasht->vector[pozitie] == NULL) {
            hasht->vector[pozitie] = nodnou;
        }
        else {
            Nod* aux = hasht->vector[pozitie];
            while (aux->next != NULL) {
                aux = aux->next;
            }
            aux->next = nodnou;
        }
    }
    return hasht;
}

void afisareHashT(HashT* hasht) {
    if (hasht != NULL) {
        for (int i = 0; i < hasht->dimensiune; i++) {
            if (hasht->vector[i] != NULL) {
                Nod* aux = hasht->vector[i];
                while (aux != NULL) {
                    printf("Pozitie: %d\n", i);
                    printf("%u %s %s %.2f %u\n",
                        aux->info->idserviciu,
                        aux->info->tipserviciu,
                        aux->info->nume,
                        aux->info->sumaplata,
                        aux->info->timpestimat);
                    aux = aux->next;
                }
            }
        }
    }
}
///functie stergere serviciu
HashT* stergereServiciu(HashT* hasht, char* tipServiciu) {
    for (int i = 0; i < hasht->dimensiune; i++) {
        Nod* aux = hasht->vector[i];
        Nod* nodAnterior = NULL;

        while (aux != NULL) {
            if (strcmp(aux->info->tipserviciu, tipServiciu) == 0) {
                Nod* nodDeSters = aux;
                if (nodAnterior == NULL) {
                    hasht->vector[i] = aux->next;
                }
                else {
                    nodAnterior->next = aux->next;
                }
                aux = aux->next;

                free(nodDeSters->info->tipserviciu);
                free(nodDeSters->info->nume);
                free(nodDeSters->info);
                free(nodDeSters);
            }
            else {
                nodAnterior = aux;
                aux = aux->next;
            }
        }
    }
    return hasht;
}
////functie eliberare memorie
void destroyHashT(HashT* hasht) {
    if (hasht != NULL) {
        for (int i = 0; i < hasht->dimensiune; i++) {
            Nod* aux = hasht->vector[i];
            while (aux != NULL) {
                Nod* temp = aux;
                aux = aux->next;
                free(temp->info->tipserviciu);
                free(temp->info->nume);
                free(temp->info);
                free(temp);
            }
        }
        free(hasht->vector);
        free(hasht);
    }
}

int main() {
    unsigned int idServiciu;
    char tipServiciu[40];
    char numeClient[40];
    float sumaPlata;
    unsigned int timpEstimat;

    HashT* hasht = initializareHashT(30);
    FILE* file = fopen("servicii.txt", "r");

    if (!file) {
        printf("Eroare la deschiderea fișierului.\n");
        return -1;
    }

    while (fscanf(file, "%u %s %s %f %u", &idServiciu, tipServiciu, numeClient, &sumaPlata, &timpEstimat) == 5) {
        Serviciu* serviciu = initializareServiciu(idServiciu, tipServiciu, numeClient, sumaPlata, timpEstimat);
        hasht = inserareinHashT(hasht, serviciu);
    }

    fclose(file);

    printf("=== Hash Table Inainte de Stergere ===\n");
    afisareHashT(hasht);

    printf("\n=== Dupa stergere a serviciilor de tip 'Transport' ===\n");
    hasht = stergereServiciu(hasht, "Transport");
    afisareHashT(hasht);

    destroyHashT(hasht);

    return 0;
}
