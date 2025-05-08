#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

// Structura Cladire
typedef struct Cladire {
    int id;
    int an_construire;
    char* denumire;
    unsigned char categorie_risc;
} Cladire;

// Structura Nod
typedef struct Nod {
    Cladire cladire;
    struct Nod* next;
} Nod;

// Structura Hash Table (HT)
typedef struct HT {
    int dim;
    Nod** tabela;
} HT;

// Funcție de citire Cladire din fișier
Cladire citirecladiredinfisier(FILE* file) {
    Cladire c;
    char buffer[100];
    char sep[4] = ".;\n";
    fgets(buffer, 100, file);
    c.id = atoi(strtok(buffer, sep));
    c.an_construire = atoi(strtok(NULL, sep));

    char* aux;
    aux = strtok(NULL, sep);
    c.denumire = (char*)malloc(strlen(aux) + 1);
    strcpy(c.denumire, aux);

    c.categorie_risc = strtok(NULL, sep)[0];
    return c;
}

// Afișare Cladire
void afisarecladire(Cladire cladire) {
    printf("Id: %d\n", cladire.id);
    printf("An_construire: %d\n", cladire.an_construire);
    printf("Denumire: %s\n", cladire.denumire);
    printf("Categorie_risc: %c\n\n", cladire.categorie_risc);
}

// Inserare Cladire în lista
void cladirenouainlista(Nod** cap, Cladire cladireNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->cladire = cladireNoua;
    nou->next = NULL;

    if (*cap == NULL) {
        *cap = nou;
    }
    else {
        Nod* p = *cap;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
}

// Inițializare Hash Table
HT* initializareHashTable(int dimensiune) {
    HT* ht = (HT*)malloc(sizeof(HT));
    ht->dim = dimensiune;
    ht->tabela = (Nod**)malloc(dimensiune * sizeof(Nod*));
    for (int i = 0; i < dimensiune; i++) {
        ht->tabela[i] = NULL;
    }
    return ht;
}

// Funcție de hash (anul construirii % dimensiune)
int hash(HT* ht, int an_construire) {
    return an_construire % ht->dim;
}

// Adăugare Cladire în Hash Table
void adaugaInHT(HT* ht, Cladire cladire) {
    int index = hash(ht, cladire.an_construire);
    cladirenouainlista(&(ht->tabela[index]), cladire);
}

// Afișare Hash Table
void afisareHT(HT* ht) {
    for (int i = 0; i < ht->dim; i++) {
        printf("Index %d:\n", i);
        Nod* p = ht->tabela[i];
        while (p) {
            afisarecladire(p->cladire);
            p = p->next;
        }
    }
}

// Main
int main() {
    HT* hashtable = initializareHashTable(7);

    FILE* file = fopen("cladiri.txt", "r");
    if (!file) {
        printf("Eroare la deschiderea fișierului!\n");
        return 1;
    }

    while (!feof(file)) {
        Cladire c = citirecladiredinfisier(file);
        adaugaInHT(hashtable, c);
    }

    fclose(file);

    afisareHT(hashtable);

    return 0;
}
