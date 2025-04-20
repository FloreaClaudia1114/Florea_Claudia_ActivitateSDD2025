#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tranzactie {
    unsigned int numarTranzactie;
    char* denumireComerciant;
    char* numeClient;
    float valoareCuDiscount;
    float discountProcentual;
    char* tipPlata;
} Tranzactie;

typedef struct Nod {
    Tranzactie* info;
    struct Nod* next;
} Nod;

typedef struct HT {
    Nod** vector;
    int dimensiune;
} HT;

Tranzactie* initializareTranzactie(unsigned int numarTranzactie, char* denumireComerciant, char* numeClient, float valoareCuDiscount, float discountProcentual, char* tipPlata) {
    Tranzactie* tranzactie = (Tranzactie*)malloc(sizeof(Tranzactie));
    tranzactie->numarTranzactie = numarTranzactie;
    tranzactie->denumireComerciant = (char*)malloc(strlen(denumireComerciant) + 1);
    strcpy(tranzactie->denumireComerciant, denumireComerciant);
    tranzactie->numeClient = (char*)malloc(strlen(numeClient) + 1);
    strcpy(tranzactie->numeClient, numeClient);
    tranzactie->valoareCuDiscount = valoareCuDiscount;
    tranzactie->discountProcentual = discountProcentual;
    tranzactie->tipPlata = (char*)malloc(strlen(tipPlata) + 1);
    strcpy(tranzactie->tipPlata, tipPlata);

    return tranzactie;
}

Nod* initializareNod(Tranzactie* info) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));

    nod->info = info;
    nod->next = NULL;

    return nod;
}

HT* initializareHT(int dimensiune) {
    HT* ht = (HT*)malloc(sizeof(HT));

    ht->dimensiune = dimensiune;
    ht->vector = (Nod**)malloc(ht->dimensiune * sizeof(Nod*));

    for (int i = 0; i < ht->dimensiune; i++) {
        ht->vector[i] = NULL;
    }

    return ht;
}

int hash(HT* ht, char* numeClient) {
    int suma = 0;
    for (int i = 0; i < strlen(numeClient); i++) {
        suma += numeClient[i];
    }
    return suma % ht->dimensiune;
}

HT* inserareInHT(HT* ht, Tranzactie* tranzactie) {
    if (ht->vector != NULL) {
        int pozitie = hash(ht, tranzactie->numeClient);
        Nod* nou = initializareNod(tranzactie);

        if (ht->vector[pozitie] == NULL) {
            ht->vector[pozitie] = nou;
        }
        else {
            Nod* aux = ht->vector[pozitie];
            while (aux->next != NULL) {
                aux = aux->next;
            }
            aux->next = nou;
        }
    }

    return ht;
}

void afisare(HT* ht) {
    if (ht != NULL) {
        for (int i = 0; i < ht->dimensiune; i++) {
            if (ht->vector[i] != NULL) {
                Nod* temp = ht->vector[i];
                while (temp != NULL) {
                    printf("Pozitie: %d \n", i);
                    printf("Numar Tranzactie: %u, Denumire Comerciant: %s, Nume Client: %s, Valoare cu Discount: %.2f, Discount Procentual: %.2f, Tip Plata: %s\n",
                        temp->info->numarTranzactie, temp->info->denumireComerciant, temp->info->numeClient, temp->info->valoareCuDiscount, temp->info->discountProcentual, temp->info->tipPlata);
                    temp = temp->next;
                }
            }
        }
    }
}

void dezalocare(HT* ht) {
    if (ht->vector != NULL) {
        for (int i = 0; i < ht->dimensiune; i++) {
            Nod* temp = ht->vector[i];
            while (temp != NULL) {
                Nod* deSters = temp;
                temp = temp->next;
                free(deSters->info->denumireComerciant);
                free(deSters->info->numeClient);
                free(deSters->info->tipPlata);
                free(deSters->info);
                free(deSters);
            }
        }
        free(ht->vector);
        free(ht);
    }
}
// functie calcul valoare medie a tranzactiilor efectuate cu cardul
float valoaremedietranzactiicard(HT* ht, char* tiptranzactie) {
    float suma = 0;
    int nrtranzactii = 0;
    for (int i = 0; i < ht->dimensiune; i++) {
        Nod* temp = ht->vector[i];
        while (temp != NULL) {
            if (strcmp(temp->info->tipPlata, tiptranzactie) == 0) {
                nrtranzactii++;
                suma += temp->info->valoareCuDiscount;

            }
            temp = temp->next;
        }

    }
    return suma / nrtranzactii;

}
//valoarea totala a discountului aplicata unui client pt toate tranzactiile efectuate pe numele sau
float valoareatotaladiscountclient(HT* ht, char* numeclient) {
    float totaldiscount = 0;
    int pozitie = hash(ht, numeclient);
    Nod* temp = ht->vector[pozitie];
    while (temp != NULL) {
        if (strcmp(temp->info->numeClient, numeclient) == 0) {
            totaldiscount += (temp->info->valoareCuDiscount * temp->info->discountProcentual / 100);

        }
        temp = temp->next;
    }
    return totaldiscount;
}

//afisam lista de clustere

void afisarelista(Nod* lista) {
    while (lista != NULL) {
        printf("Numar Tranzactie: %u, Denumire Comerciant: %s, Nume Client: %s, Valoare cu Discount: %.2f, Discount Procentual: %.2f, Tip Plata: %s\n",
            lista->info->numarTranzactie, lista->info->denumireComerciant, lista->info->numeClient, lista->info->valoareCuDiscount, lista->info->discountProcentual, lista->info->tipPlata);
        lista = lista->next;
    }

}

//functia pt determinarea celei mai lungi liste de coliziuni

Nod* clustermaxim(HT* ht) {
    Nod* maxim = NULL;
    int lungimemaximacluster = 0;
    for (int i = 0; i < ht->dimensiune; i++) {
        Nod* temp = ht->vector[i];
        int lungimecurenta = 0;
        while (temp != NULL) {
            lungimecurenta++;
            temp = temp->next;

        }
        if (lungimecurenta > lungimemaximacluster) {
            lungimemaximacluster = lungimecurenta;
            maxim = ht->vector[i];
        }

    }

    return maxim;
}

int main() {
    unsigned int numarTranzactie;
    char denumireComerciant[50];
    char numeClient[50];
    float valoareCuDiscount;
    float discountProcentual;
    char tipPlata[10];

    HT* ht = initializareHT(20);
    FILE* f = fopen("tranzactii.txt", "r");


    while (fscanf(f, "%u %49s %49s %f %f %9s", &numarTranzactie, denumireComerciant, numeClient, &valoareCuDiscount, &discountProcentual, tipPlata) == 6) {
        Tranzactie* tranzactie = initializareTranzactie(numarTranzactie, denumireComerciant, numeClient, valoareCuDiscount, discountProcentual, tipPlata);
        ht = inserareInHT(ht, tranzactie);
    }

    fclose(f);
    afisare(ht);
    printf("==============================================\n");
    //afisam functia de calcul medie pe tiptranzactie

    float medieplatacard = valoaremedietranzactiicard(ht, "card");
    printf("%5.2f\n", medieplatacard);

    printf("==============================================\n");
    //afisam valoareatotaldiscount

    float discounttotal = valoareatotaladiscountclient(ht, "Popescu");
    printf("%5.2f", discounttotal);

    printf("==============================================\n");
    // afisam clustermaxim

    Nod* maxcluster = clustermaxim(ht);
    afisarelista(maxcluster);



}
