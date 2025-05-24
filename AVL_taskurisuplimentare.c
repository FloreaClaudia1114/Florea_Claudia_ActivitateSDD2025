#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Farmacie {
    unsigned short int idFarmacie;
    char* denumireFarmacie;
    float cifraAfaceriAnuala;
    char* numeFarmacistSef;
    unsigned char nrAngajati;
    char* localitate;
} Farmacie;

typedef struct NodABC {
    Farmacie* info;
    struct NodABC* left;
    struct NodABC* right;
} NodABC;


typedef struct NodListaSimpla {
    Farmacie* info;
    struct NodListaSimpla* next;
} NodListaSimpla;

typedef struct NodListaDublu {
    Farmacie* info;
    struct NodListaDublu* prev;
    struct NodListaDublu* next;
} NodListaDublu;



Farmacie* initializareFarmacie(unsigned short int idFarmacie, char* denumireFarmacie, float cifraAfaceriAnuala, char* numeFarmacistSef, unsigned char nrAngajati, char* localitate) {
    Farmacie* farmacie = (Farmacie*)malloc(sizeof(Farmacie));
    if (!farmacie) {
        perror("Eroare alocare memorie farmacie");
        return NULL;
    }

    farmacie->idFarmacie = idFarmacie;

    farmacie->denumireFarmacie = (char*)malloc(strlen(denumireFarmacie) + 1);
    if (!farmacie->denumireFarmacie) {
        free(farmacie);
        perror("Eroare alocare memorie denumireFarmacie");
        return NULL;
    }
    strcpy(farmacie->denumireFarmacie, denumireFarmacie);

    farmacie->cifraAfaceriAnuala = cifraAfaceriAnuala;

    farmacie->numeFarmacistSef = (char*)malloc(strlen(numeFarmacistSef) + 1);
    if (!farmacie->numeFarmacistSef) {
        free(farmacie->denumireFarmacie);
        free(farmacie);
        perror("Eroare alocare memorie numeFarmacistSef");
        return NULL;
    }
    strcpy(farmacie->numeFarmacistSef, numeFarmacistSef);

    farmacie->nrAngajati = nrAngajati;

    farmacie->localitate = (char*)malloc(strlen(localitate) + 1);
    if (!farmacie->localitate) {
        free(farmacie->numeFarmacistSef);
        free(farmacie->denumireFarmacie);
        free(farmacie);
        perror("Eroare alocare memorie localitate");
        return NULL;
    }
    strcpy(farmacie->localitate, localitate);

    return farmacie;
}

NodABC* initializareNod(Farmacie* farmacie) {
    NodABC* nodNou = (NodABC*)malloc(sizeof(NodABC));
    if (!nodNou) {
        perror("Eroare alocare memorie nod");
        return NULL;
    }
    nodNou->info = farmacie;
    nodNou->left = NULL;
    nodNou->right = NULL;
    return nodNou;
}



int inaltime(NodABC* radacina) {
    if (radacina == NULL)
        return 0;
    int hst = inaltime(radacina->left);
    int hdt = inaltime(radacina->right);
    return (hst > hdt ? hst : hdt) + 1;
}

int gradechilibru(NodABC* radacina) {
    if (radacina == NULL)
        return 0;
    return inaltime(radacina->left) - inaltime(radacina->right);
}

NodABC* rotiredreapta(NodABC* y) {
    NodABC* x = y->left;
    NodABC* T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}

NodABC* rotirestanga(NodABC* x) {
    NodABC* y = x->right;
    NodABC* T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}

NodABC* inserareAVL(NodABC* radacina, Farmacie* farmacie) {
    if (radacina == NULL)
        return initializareNod(farmacie);
    if (farmacie->idFarmacie < radacina->info->idFarmacie)
        radacina->left = inserareAVL(radacina->left, farmacie);
    else if (farmacie->idFarmacie > radacina->info->idFarmacie)
        radacina->right = inserareAVL(radacina->right, farmacie);
    else
        return radacina;

    int balance = gradechilibru(radacina);

    if (balance > 1 && farmacie->idFarmacie < radacina->left->info->idFarmacie)
        return rotiredreapta(radacina);
    if (balance < -1 && farmacie->idFarmacie > radacina->right->info->idFarmacie)
        return rotirestanga(radacina);
    if (balance > 1 && farmacie->idFarmacie > radacina->left->info->idFarmacie) {
        radacina->left = rotirestanga(radacina->left);
        return rotiredreapta(radacina);
    }
    if (balance < -1 && farmacie->idFarmacie < radacina->right->info->idFarmacie) {
        radacina->right = rotiredreapta(radacina->right);
        return rotirestanga(radacina);
    }

    return radacina;
}


Farmacie* cautaFarmacie(NodABC* radacina, unsigned short int idFarmacie) {
    if (!radacina)
        return NULL;
    if (radacina->info->idFarmacie == idFarmacie)
        return radacina->info;
    if (idFarmacie < radacina->info->idFarmacie)
        return cautaFarmacie(radacina->left, idFarmacie);
    else
        return cautaFarmacie(radacina->right, idFarmacie);
}

void salvareInVector(NodABC* radacina, Farmacie** vector, int* index, char tip) {
    if (!radacina) return;
    if (tip == 'P') vector[(*index)++] = radacina->info;
    salvareInVector(radacina->left, vector, index, tip);
    if (tip == 'I') vector[(*index)++] = radacina->info;
    salvareInVector(radacina->right, vector, index, tip);
    if (tip == 'O') vector[(*index)++] = radacina->info;
}

void salvareInListaSimpla(NodABC* radacina, NodListaSimpla** cap, NodListaSimpla** coada, char tip) {
    if (!radacina) return;
    if (tip == 'P') {
        NodListaSimpla* nod = (NodListaSimpla*)malloc(sizeof(NodListaSimpla));
        nod->info = radacina->info;
        nod->next = NULL;
        if (!(*cap)) *cap = *coada = nod;
        else { (*coada)->next = nod; *coada = nod; }
    }
    salvareInListaSimpla(radacina->left, cap, coada, tip);
    if (tip == 'I') {
        NodListaSimpla* nod = (NodListaSimpla*)malloc(sizeof(NodListaSimpla));
        nod->info = radacina->info;
        nod->next = NULL;
        if (!(*cap)) *cap = *coada = nod;
        else { (*coada)->next = nod; *coada = nod; }
    }
    salvareInListaSimpla(radacina->right, cap, coada, tip);
    if (tip == 'O') {
        NodListaSimpla* nod = (NodListaSimpla*)malloc(sizeof(NodListaSimpla));
        nod->info = radacina->info;
        nod->next = NULL;
        if (!(*cap)) *cap = *coada = nod;
        else { (*coada)->next = nod; *coada = nod; }
    }
}


void salvareInListaDubla(NodABC* radacina, NodListaDublu** cap, NodListaDublu** coada, char tip) {
    if (!radacina) return;
    if (tip == 'P') {
        NodListaDublu* nod = (NodListaDublu*)malloc(sizeof(NodListaDublu));
        nod->info = radacina->info;
        nod->next = NULL;
        nod->prev = *coada;
        if (*coada) (*coada)->next = nod;
        else *cap = nod;
        *coada = nod;
    }
    salvareInListaDubla(radacina->left, cap, coada, tip);
    if (tip == 'I') {
        NodListaDublu* nod = (NodListaDublu*)malloc(sizeof(NodListaDublu));
        nod->info = radacina->info;
        nod->next = NULL;
        nod->prev = *coada;
        if (*coada) (*coada)->next = nod;
        else *cap = nod;
        *coada = nod;
    }
    salvareInListaDubla(radacina->right, cap, coada, tip);
    if (tip == 'O') {
        NodListaDublu* nod = (NodListaDublu*)malloc(sizeof(NodListaDublu));
        nod->info = radacina->info;
        nod->next = NULL;
        nod->prev = *coada;
        if (*coada) (*coada)->next = nod;
        else *cap = nod;
        *coada = nod;
    }
}


void afisarePreordine(NodABC* radacina) {
    if (!radacina) return;
    printf("%hu %s %f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
        radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef,
        radacina->info->nrAngajati, radacina->info->localitate);
    afisarePreordine(radacina->left);
    afisarePreordine(radacina->right);
}

void afisareInordine(NodABC* radacina) {
    if (!radacina) return;
    afisareInordine(radacina->left);
    printf("%hu %s %f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
        radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef,
        radacina->info->nrAngajati, radacina->info->localitate);
    afisareInordine(radacina->right);
}

void afisarePostordine(NodABC* radacina) {
    if (!radacina) return;
    afisarePostordine(radacina->left);
    afisarePostordine(radacina->right);
    printf("%hu %s %f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
        radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef,
        radacina->info->nrAngajati, radacina->info->localitate);
}



int numaraNoduri(NodABC* radacina) {
    if (!radacina)
        return 0;
    return 1 + numaraNoduri(radacina->left) + numaraNoduri(radacina->right);
}



void salvareInVectorPreordine(NodABC* radacina, Farmacie** vector, int* index) {
    if (!radacina) return;
    vector[(*index)++] = radacina->info;
    salvareInVectorPreordine(radacina->left, vector, index);
    salvareInVectorPreordine(radacina->right, vector, index);
}



typedef NodListaSimpla NodLS;

void salvareInListaSimplaInordine(NodABC* radacina, NodLS** cap) {
    if (!radacina) return;
    salvareInListaSimplaInordine(radacina->left, cap);
    NodLS* nod = (NodLS*)malloc(sizeof(NodLS));
    nod->info = radacina->info;
    nod->next = *cap;
    *cap = nod;
    salvareInListaSimplaInordine(radacina->right, cap);
}



void dezalocareListaSimpla(NodLS* cap) {
    while (cap) {
        NodLS* temp = cap;
        cap = cap->next;
        free(temp);
    }
}



typedef NodListaDublu NodLD;

void salvareInListaDublaPostordine(NodABC* radacina, NodLD** cap, NodLD** coada) {
    if (!radacina) return;
    salvareInListaDublaPostordine(radacina->left, cap, coada);
    salvareInListaDublaPostordine(radacina->right, cap, coada);
    NodLD* nod = (NodLD*)malloc(sizeof(NodLD));
    nod->info = radacina->info;
    nod->prev = *coada;
    nod->next = NULL;
    if (*coada) (*coada)->next = nod;
    else *cap = nod;
    *coada = nod;
}



void dezalocareListaDubla(NodLD* cap) {
    while (cap) {
        NodLD* temp = cap;
        cap = cap->next;
        free(temp);
    }
}



void dezalocareFarmacie(Farmacie* farmacie) {
    if (!farmacie) return;
    free(farmacie->denumireFarmacie);
    free(farmacie->numeFarmacistSef);
    free(farmacie->localitate);
    free(farmacie);
}



void dezalocareArbore(NodABC* radacina) {
    if (!radacina) return;
    dezalocareArbore(radacina->left);
    dezalocareArbore(radacina->right);
    dezalocareFarmacie(radacina->info);
    free(radacina);
}



NodABC* citireDinFisier(char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare la deschiderea fisierului");
        return NULL;
    }

    NodABC* radacina = NULL;
    unsigned short int id;
    char bufferDenumire[100], bufferNumeSef[100], bufferLocalitate[100];
    float cifra;
    unsigned char nrAng;

    while (fscanf(f, "%hu %99s %f %99s %hhu %99s", &id, bufferDenumire, &cifra, bufferNumeSef, &nrAng, bufferLocalitate) == 6) {
        Farmacie* fNoua = initializareFarmacie(id, bufferDenumire, cifra, bufferNumeSef, nrAng, bufferLocalitate);
        if (fNoua == NULL) {
            fclose(f);
            dezalocareArbore(radacina);
            return NULL;
        }
        radacina = inserareAVL(radacina, fNoua);
    }
    fclose(f);
    return radacina;
}


int main() {
    NodABC* radacina = citireDinFisier("farmacii.txt");
    if (!radacina) {
        printf("Nu s-au putut citi datele din fisier.\n");
        return 1;
    }

    printf("Afisare arbore PREORDINE:\n");
    afisarePreordine(radacina);
    printf("\nAfisare arbore INORDINE:\n");
    afisareInordine(radacina);
    printf("\nAfisare arbore POSTORDINE:\n");
    afisarePostordine(radacina);

    // Numar noduri pentru alocare vector
    int nrNoduri = numaraNoduri(radacina);

    // Vector Farmacie*
    Farmacie** vector = (Farmacie**)malloc(nrNoduri * sizeof(Farmacie*));
    int index = 0;
    salvareInVectorPreordine(radacina, vector, &index);

    printf("\nAfisare vector (preordine):\n");
    for (int i = 0; i < nrNoduri; i++) {
        printf("%hu %s %f %s %hhu %s\n",
            vector[i]->idFarmacie, vector[i]->denumireFarmacie, vector[i]->cifraAfaceriAnuala,
            vector[i]->numeFarmacistSef, vector[i]->nrAngajati, vector[i]->localitate);
    }

    // Lista simpla (inordine)
    NodListaSimpla* capLS = NULL;
    salvareInListaSimplaInordine(radacina, &capLS);
    printf("\nAfisare lista simpla (inordine):\n");
    for (NodListaSimpla* p = capLS; p != NULL; p = p->next) {
        printf("%hu %s %f %s %hhu %s\n",
            p->info->idFarmacie, p->info->denumireFarmacie, p->info->cifraAfaceriAnuala,
            p->info->numeFarmacistSef, p->info->nrAngajati, p->info->localitate);
    }

    // Lista dubla (postordine)
    NodListaDublu* capLD = NULL;
    NodListaDublu* coadaLD = NULL;
    salvareInListaDublaPostordine(radacina, &capLD, &coadaLD);
    printf("\nAfisare lista dubla (postordine):\n");
    for (NodListaDublu* p = capLD; p != NULL; p = p->next) {
        printf("%hu %s %f %s %hhu %s\n",
            p->info->idFarmacie, p->info->denumireFarmacie, p->info->cifraAfaceriAnuala,
            p->info->numeFarmacistSef, p->info->nrAngajati, p->info->localitate);
    }

    // Dezalocari
    free(vector);
    dezalocareListaSimpla(capLS);
    dezalocareListaDubla(capLD);
    dezalocareArbore(radacina);

    return 0;
}

