#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct Rezervare {
    unsigned int idRezervare;
    char* numeClient;
    char* tipCamera;
    unsigned char numarNopti;
    float pretPerNoapte;
} Rezervare;

typedef struct NodRez {
    Rezervare* info;
    struct NodRez* next;
} NodRez;

typedef struct {
    NodRez** vector;
    int dimensiune;
} HTR; 

HTR* initializareHTRez(int dimensiune) {
    HTR* ht = (HTR*)malloc(sizeof(HTR));
    ht->dimensiune = dimensiune;
    ht->vector = (NodRez**)malloc(dimensiune * sizeof(NodRez*));
    for (int i = 0; i < dimensiune; i++) {
        ht->vector[i] = NULL;
    }
    return ht;
}

int hashRez(HTR* ht, char* numeClient) {
    int suma = 0;
    for (int i = 0; i < strlen(numeClient); i++) {
        suma += numeClient[i];
    }
    return suma % ht->dimensiune;
}

Rezervare* initializareRezervare(unsigned int idRezervare, char* numeClient, char* tipCamera, unsigned char numarNopti, float pretPerNoapte) {
    Rezervare* rez = (Rezervare*)malloc(sizeof(Rezervare));
    rez->idRezervare = idRezervare;

    rez->numeClient = (char*)malloc(strlen(numeClient) + 1);
    strncpy(rez->numeClient, numeClient, strlen(numeClient) + 1);

    rez->tipCamera = (char*)malloc(strlen(tipCamera) + 1);
    strncpy(rez->tipCamera, tipCamera, strlen(tipCamera) + 1);

    rez->numarNopti = numarNopti;
    rez->pretPerNoapte = pretPerNoapte;

    return rez;
}

NodRez* initializareNodRez(Rezervare* info) {
    NodRez* nod = (NodRez*)malloc(sizeof(NodRez));
    nod->info = info;
    nod->next = NULL;
    return nod;
}

HTR* inserareInHTRez(HTR* ht, Rezervare* rez) {
    if (ht->vector != NULL) {
        int pozitie = hashRez(ht, rez->numeClient);
        NodRez* nou = initializareNodRez(rez);

        if (ht->vector[pozitie] == NULL) {
            ht->vector[pozitie] = nou;
        }
        else {
            NodRez* aux = ht->vector[pozitie];
            while (aux->next != NULL) {
                aux = aux->next;
            }
            aux->next = nou;
        }
    }
    return ht;
}

void afisareHTRez(HTR* ht) {
    if (ht != NULL) {
        for (int i = 0; i < ht->dimensiune; i++) {
            NodRez* temp = ht->vector[i];
            while (temp != NULL) {
                printf("Pozitie: %d\n", i);
                printf("ID Rezervare: %u, Client: %s, Tip Camera: %s, Nopti: %d, Pret/Noapte: %.2f\n",
                    temp->info->idRezervare,
                    temp->info->numeClient,
                    temp->info->tipCamera,
                    temp->info->numarNopti,
                    temp->info->pretPerNoapte);
                temp = temp->next;
            }
        }
    }
}

void dezalocareHTRez(HTR* ht) {
    if (ht != NULL) {
        for (int i = 0; i < ht->dimensiune; i++) {
            NodRez* temp = ht->vector[i];
            while (temp != NULL) {
                NodRez* deSters = temp;
                temp = temp->next;
                free(deSters->info->numeClient);
                free(deSters->info->tipCamera);
                free(deSters->info);
                free(deSters);
            }
        }
        free(ht->vector);
        free(ht);
    }
}

float valoareTotalaRezervare(Rezervare* rez) {
    if (rez != NULL) {
        return rez->numarNopti * rez->pretPerNoapte;
    }
    return 0.0f;
}

float totalIncasariClient(HTR* ht, char* numeClient) {
    float total = 0;
    for (int i = 0; i < ht->dimensiune; i++) {
        NodRez* temp = ht->vector[i];
        while (temp != NULL) {
            if (strcmp(temp->info->numeClient, numeClient) == 0) {
                total += valoareTotalaRezervare(temp->info);
            }
            temp = temp->next;
        }
    }
    return total;
}

int main() {
    HTR* htRez = initializareHTRez(10);

    Rezervare* r1 = initializareRezervare(1, "Popescu", "Single", 3, 100.0f);
    Rezervare* r2 = initializareRezervare(2, "Ionescu", "Double", 2, 150.0f);
    Rezervare* r3 = initializareRezervare(3, "Popescu", "Suite", 1, 300.0f);

    htRez = inserareInHTRez(htRez, r1);
    htRez = inserareInHTRez(htRez, r2);
    htRez = inserareInHTRez(htRez, r3);

    afisareHTRez(htRez);

    char client[] = "Popescu";
    float incasari = totalIncasariClient(htRez, client);
    printf("\nTotal incasari pentru %s: %.2f\n", client, incasari);

    dezalocareHTRez(htRez);
    return 0;
}

