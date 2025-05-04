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

Farmacie* initializareFarmacie(unsigned short int idFarmacie, char* denumireFarmacie, float cifraAfaceriAnuala, char* numeFarmacistSef, unsigned char nrAngajati, char* localitate) {
    Farmacie* farmacie = (Farmacie*)malloc(sizeof(Farmacie));
    farmacie->idFarmacie = idFarmacie;
    farmacie->denumireFarmacie = (char*)malloc(strlen(denumireFarmacie) + 1);
    strcpy(farmacie->denumireFarmacie, denumireFarmacie);
    farmacie->cifraAfaceriAnuala = cifraAfaceriAnuala;
    farmacie->numeFarmacistSef = (char*)malloc(strlen(numeFarmacistSef) + 1);
    strcpy(farmacie->numeFarmacistSef, numeFarmacistSef);
    farmacie->nrAngajati = nrAngajati;
    farmacie->localitate = (char*)malloc(strlen(localitate) + 1);
    strcpy(farmacie->localitate, localitate);
    return farmacie;
}

NodABC* initializareNod(Farmacie* farmacie) {
    NodABC* nodNou = (NodABC*)malloc(sizeof(NodABC));
    nodNou->info = farmacie;
    nodNou->left = NULL;
    nodNou->right = NULL;
    return nodNou;
}

NodABC* inserareArbore(NodABC* radacina, Farmacie* farmacie) {
    if (radacina == NULL) {
        return initializareNod(farmacie);
    }
    if (farmacie->idFarmacie < radacina->info->idFarmacie) {
        radacina->left = inserareArbore(radacina->left, farmacie);
    }
    else if (farmacie->idFarmacie > radacina->info->idFarmacie) {
        radacina->right = inserareArbore(radacina->right, farmacie);
    }
    return radacina;
}

void afisarePreordine(NodABC* radacina) {
    if (radacina != NULL) {
        printf("%hu %s %.2f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef,
            radacina->info->nrAngajati, radacina->info->localitate);
        afisarePreordine(radacina->left);
        afisarePreordine(radacina->right);
    }
}

void afisareInordine(NodABC* radacina) {
    if (radacina != NULL) {
        afisareInordine(radacina->left);
        printf("%hu %s %.2f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef,
            radacina->info->nrAngajati, radacina->info->localitate);
        afisareInordine(radacina->right);
    }
}

void afisarePostordine(NodABC* radacina) {
    if (radacina != NULL) {
        afisarePostordine(radacina->left);
        afisarePostordine(radacina->right);
        printf("%hu %s %.2f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef,
            radacina->info->nrAngajati, radacina->info->localitate);
    }
}

void dezalocareArbore(NodABC* radacina) {
    if (radacina != NULL) {
        dezalocareArbore(radacina->left);
        dezalocareArbore(radacina->right);
        free(radacina->info->denumireFarmacie);
        free(radacina->info->numeFarmacistSef);
        free(radacina->info->localitate);
        free(radacina->info);
        free(radacina);
    }
}

int nrTotalAngajatiInLocalitate(NodABC* radacina, char* localitate) {
    int total = 0;
    if (radacina != NULL) {
        if (strcmp(radacina->info->localitate, localitate) == 0) {
            total += radacina->info->nrAngajati;
        }
        total += nrTotalAngajatiInLocalitate(radacina->left, localitate);
        total += nrTotalAngajatiInLocalitate(radacina->right, localitate);
    }
    return total;
}

float cifraMedieLunara(NodABC* radacina, unsigned short int idFarmacie) {
    if (radacina == NULL) {
        return 0;
    }
    if (radacina->info->idFarmacie == idFarmacie) {
        return radacina->info->cifraAfaceriAnuala / 12.0f;
    }

    float valoare = cifraMedieLunara(radacina->left, idFarmacie);
    if (valoare != 0) {
        return valoare;
    }
    return cifraMedieLunara(radacina->right, idFarmacie);
}

NodABC* stergerePeNivel(NodABC* radacina, int nivelCurent, int nivelSpecificat) {
    if (radacina == NULL) return NULL;

    if (nivelCurent == nivelSpecificat) {
        if (radacina->left == NULL && radacina->right == NULL) {
            free(radacina->info->denumireFarmacie);
            free(radacina->info->numeFarmacistSef);
            free(radacina->info->localitate);
            free(radacina->info);
            free(radacina);
            return NULL;
        }
    }
    else {
        radacina->left = stergerePeNivel(radacina->left, nivelCurent + 1, nivelSpecificat);
        radacina->right = stergerePeNivel(radacina->right, nivelCurent + 1, nivelSpecificat);
    }
    return radacina;
}

NodABC* minim(NodABC* radacina) {
    while (radacina != NULL && radacina->left != NULL) {
        radacina = radacina->left;
    }
    return radacina;
}

NodABC* stergereRadacina(NodABC* radacina) {
    if (radacina == NULL) return NULL;

    NodABC* temp;
    if (radacina->left == NULL) {
        temp = radacina->right;
    }
    else if (radacina->right == NULL) {
        temp = radacina->left;
    }
    else {
        NodABC* succesor = minim(radacina->right);

        radacina->info->idFarmacie = succesor->info->idFarmacie;

        free(radacina->info->denumireFarmacie);
        radacina->info->denumireFarmacie = (char*)malloc(strlen(succesor->info->denumireFarmacie) + 1);
        strcpy(radacina->info->denumireFarmacie, succesor->info->denumireFarmacie);

        radacina->info->cifraAfaceriAnuala = succesor->info->cifraAfaceriAnuala;

        free(radacina->info->numeFarmacistSef);
        radacina->info->numeFarmacistSef = (char*)malloc(strlen(succesor->info->numeFarmacistSef) + 1);
        strcpy(radacina->info->numeFarmacistSef, succesor->info->numeFarmacistSef);

        radacina->info->nrAngajati = succesor->info->nrAngajati;

        free(radacina->info->localitate);
        radacina->info->localitate = (char*)malloc(strlen(succesor->info->localitate) + 1);

        strcpy(radacina->info->localitate, succesor->info->localitate);
        radacina->right = stergereRadacina(radacina->right);
        return radacina;
    }

    free(radacina->info->denumireFarmacie);
    free(radacina->info->numeFarmacistSef);
    free(radacina->info->localitate);
    free(radacina->info);
    free(radacina);

    return temp;
}

int main() {
    unsigned short int idFarmacie;
    char denumireFarmacie[100];
    float cifraAfaceriAnuala;
    char numeFarmacistSef[100];
    unsigned char nrAngajati;
    char localitate[100];

    NodABC* radacina = NULL;

    FILE* file = fopen("farmacii.txt", "r");
    if (!file) {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }

    while (fscanf(file, "%hu %s %f %s %hhu %s", &idFarmacie, denumireFarmacie, &cifraAfaceriAnuala,
        numeFarmacistSef, &nrAngajati, localitate) == 6) {
        Farmacie* farmacie = initializareFarmacie(idFarmacie, denumireFarmacie, cifraAfaceriAnuala,
            numeFarmacistSef, nrAngajati, localitate);
        radacina = inserareArbore(radacina, farmacie);
    }
    fclose(file);

    printf("Afisare preordine:\n");
    afisarePreordine(radacina);

    int totalAngajati = nrTotalAngajatiInLocalitate(radacina, "Bucuresti");
    printf("Numar total angajati in Bucuresti: %d\n", totalAngajati);

    float cifralunara = cifraMedieLunara(radacina, 5);
    if (cifralunara == 0) {
        printf("Farmacia cu ID 5 nu a fost gasita.\n");
    }
    else {
        printf("Cifra medie lunara pentru farmacia 5: %.2f\n", cifralunara);
    }

    // Exemplu: radacina = stergerePeNivel(radacina, 0, 2);
    // afisareInordine(radacina);

    radacina = stergereRadacina(radacina);
    printf("\nDupa stergerea radacinii:\n");
    afisareInordine(radacina);

    dezalocareArbore(radacina);
    return 0;
}
