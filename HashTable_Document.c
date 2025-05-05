#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Structura Document
typedef struct Document {
    char* denumire;
    char* cale;
    char* extensie;
    unsigned int dimensiune;
} Document;

// Structura ListaDocumente (nod din lista înlănțuită)
typedef struct ListaDocumente {
    Document document;
    struct ListaDocumente* next;
} ListaDocumente;

// Structura TabelaDocumente (hash table)
typedef struct TabelaDocumente {
    int dim;
    ListaDocumente** tabela;
} TabelaDocumente;

// Citirea unui document din fișier
Document citireDocument(FILE* f) {
    char buffer[100];
    Document doc = { NULL, NULL, NULL, 0 };

    if (fgets(buffer, sizeof(buffer), f)) {
        char* token = strtok(buffer, ",;\n");
        if (token == NULL) return doc;
        doc.denumire = (char*)malloc(strlen(token) + 1);
        strcpy(doc.denumire, token);

        token = strtok(NULL, ",;\n");
        if (token == NULL) return doc;
        doc.cale = (char*)malloc(strlen(token) + 1);
        strcpy(doc.cale, token);

        token = strtok(NULL, ",;\n");
        if (token == NULL) return doc;
        doc.extensie = (char*)malloc(strlen(token) + 1);
        strcpy(doc.extensie, token);

        token = strtok(NULL, ",;\n");
        if (token == NULL) return doc;
        doc.dimensiune = atoi(token);
    }

    return doc;
}

// Afișarea unui document
void afisareDocument(Document doc) {
    printf("Denumire: %s\n", doc.denumire);
    printf("Cale: %s\n", doc.cale);
    printf("Extensie: %s\n", doc.extensie);
    printf("Dimensiune: %d\n", doc.dimensiune);
}

// Afișarea listei de documente
void afisareListaDocumente(ListaDocumente* cap) {
    while (cap) {
        afisareDocument(cap->document);
        cap = cap->next;
    }
}

// Adăugarea unui document în listă
void adaugaDocumentInLista(ListaDocumente* cap, Document docNou) {
    ListaDocumente* p = cap;
    while (p->next) {
        p = p->next;
    }

    ListaDocumente* nou = (ListaDocumente*)malloc(sizeof(ListaDocumente));
    nou->document = docNou;
    nou->next = NULL;
    p->next = nou;
}

// Inițializare tabelă hash
TabelaDocumente initializareTabelaDocumente(int dim) {
    TabelaDocumente tabela;
    tabela.dim = dim;
    tabela.tabela = (ListaDocumente**)malloc(dim * sizeof(ListaDocumente*));
    for (int i = 0; i < dim; i++) {
        tabela.tabela[i] = NULL;
    }
    return tabela;
}

// Funcția de hash
int calculeazaHash(const char* cheie, int dim) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(cheie); i++) {
        hash = hash * 31 + (unsigned char)cheie[i];
    }
    return hash % dim;
}

// Inserare document în tabelă
void inserareDocumentInTabela(TabelaDocumente tabela, Document doc) {
    int pozitie = calculeazaHash(doc.denumire, tabela.dim);
    if (tabela.tabela[pozitie] == NULL) {
        tabela.tabela[pozitie] = (ListaDocumente*)malloc(sizeof(ListaDocumente));
        tabela.tabela[pozitie]->document = doc;
        tabela.tabela[pozitie]->next = NULL;
    }
    else {
        adaugaDocumentInLista(tabela.tabela[pozitie], doc);
    }
}

// Citire documente din fișier și inserare în tabelă
TabelaDocumente citireDocumenteDinFisier(const char* numeFisier, int dimensiune) {
    TabelaDocumente tabela = initializareTabelaDocumente(dimensiune);
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fișierului.\n");
        return tabela;
    }

    while (!feof(f)) {
        Document doc = citireDocument(f);
        if (doc.denumire != NULL) {
            inserareDocumentInTabela(tabela, doc);
        }
    }

    fclose(f);
    return tabela;
}

// Afișare tabelă documente
void afisareTabelaDocumente(TabelaDocumente tabela) {
    for (int i = 0; i < tabela.dim; i++) {
        if (tabela.tabela[i] != NULL) {
            printf("Documentele de pe poziția %d sunt:\n", i);
            afisareListaDocumente(tabela.tabela[i]);
        }
        else {
            printf("Pe poziția %d nu există documente.\n", i);
        }
    }
}

// Dezalocare listă de documente
void dezalocareListaDocumente(ListaDocumente** cap) {
    ListaDocumente* p = *cap;
    while (p) {
        ListaDocumente* temp = p;
        p = p->next;

        if (temp->document.denumire) free(temp->document.denumire);
        if (temp->document.cale) free(temp->document.cale);
        if (temp->document.extensie) free(temp->document.extensie);

        free(temp);
    }
    *cap = NULL;
}

// Dezalocare tabelă
void dezalocareTabelaDocumente(TabelaDocumente* tabela) {
    for (int i = 0; i < tabela->dim; i++) {
        dezalocareListaDocumente(&(tabela->tabela[i]));
    }

    free(tabela->tabela);
    tabela->tabela = NULL;
    tabela->dim = 0;
}

// Caută un document după denumire în tabela hash
Document* cautaDocumentDupaDenumire(TabelaDocumente tabela, const char* denumire) {
    int pozitie = calculeazaHash(denumire, tabela.dim);
    ListaDocumente* p = tabela.tabela[pozitie];

    while (p) {
        if (strcmp(p->document.denumire, denumire) == 0) {
            return &(p->document);
        }
        p = p->next;
    }

    return NULL;
}



int main() {
    TabelaDocumente tabela = citireDocumenteDinFisier("fisier.txt", 7);

    printf("\n========= Tabela de documente =========\n");
    afisareTabelaDocumente(tabela);



    const char* cautat = "poza_vacanta";
    Document* gasit = cautaDocumentDupaDenumire(tabela, cautat);

    if (gasit) {
        printf("\n===== Document găsit: %s =====\n", cautat);
        afisareDocument(*gasit);
    }
    else {
        printf("\nDocumentul cu denumirea '%s' nu a fost găsit.\n", cautat);
    }


    return 0;

    dezalocareTabelaDocumente(&tabela);

    printf("\n========= Tabela după dezalocare =========\n");
    afisareTabelaDocumente(tabela);
}
