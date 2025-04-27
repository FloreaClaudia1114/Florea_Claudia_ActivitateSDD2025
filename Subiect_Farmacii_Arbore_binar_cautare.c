#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Farmacie {
    unsigned short int idFarmacie;
    char* denumireFarmacie;
    float cifraAfaceriAnuala;
    char* numeFarmacistSef;
    unsigned char nrAngajati;
    char* localitate;
}Farmacie;
//doi pointeri unul pt stanga si unul pt dreapta
typedef struct NodABC {
    Farmacie* info;
    struct NodABC* left;
    struct NodABC* right;
}NodABC;

//functia de initializare a unei farmacii
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

// functia de initializare a unui nod

NodABC* initializarenod(Farmacie* farmacie) {
    NodABC* nodnou = (NodABC*)malloc(sizeof(NodABC));
    nodnou->info = farmacie;
    nodnou->left = NULL;
    nodnou->right = NULL;

    return nodnou;
}
//functia de inserare in arbore

NodABC* inserarearbore(NodABC* radacina, Farmacie* farmacie) {
    if (radacina == NULL) {
        return initializarenod(farmacie);
    }
    if (farmacie->idFarmacie < radacina->info->idFarmacie) {
        radacina->left = inserarearbore(radacina->left, farmacie);
    }
    if (farmacie->idFarmacie > radacina->info->idFarmacie) {
        radacina->right = inserarearbore(radacina->right, farmacie);
    }

    return radacina;

}
//afisarile toate trei, preordine, radacina capul arborelui

void afisarepreordine(NodABC* radacina) {
    if (radacina != NULL) {
        printf("%hu %s %.2f %s %hhu %s\n",
            radacina->info->idFarmacie,
            radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala,
            radacina->info->numeFarmacistSef,
            radacina->info->nrAngajati,
            radacina->info->localitate);

        afisarepreordine(radacina->left);
        afisarepreordine(radacina->right);
    }
}

void afisareinordine(NodABC* radacina) {
    if (radacina != NULL) {
        afisareinordine(radacina->left);

        printf("%hu %s %.2f %s %hhu %s\n",
            radacina->info->idFarmacie,
            radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala,
            radacina->info->numeFarmacistSef,
            radacina->info->nrAngajati,
            radacina->info->localitate);

        afisareinordine(radacina->right);
    }
}
void afisarepostordine(NodABC* radacina) {

    if (radacina != NULL) {
        afisarepostordine(radacina->left);
        afisarepostordine(radacina->right);

        printf("%hu %s %.2f %s %hhu %s\n",
            radacina->info->idFarmacie,
            radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala,
            radacina->info->numeFarmacistSef,
            radacina->info->nrAngajati,
            radacina->info->localitate);
    }
}
///dezalocare arbore,sensul ar fi postordine cel mai simplu
void dezalocarearbore(NodABC* radacina) {
    if (radacina != NULL) {
        dezalocarearbore(radacina->left);
        dezalocarearbore(radacina->right);

        free(radacina->info->denumireFarmacie);
        free(radacina->info->numeFarmacistSef);
        free(radacina->info->localitate);
        free(radacina->info);
        free(radacina);


    }
}
int main() {
    unsigned short int idFarmacie;
    char denumireFarmacie[20];
    float cifraAfaceriAnuala;
    char numeFarmacistSef[20];
    unsigned char nrAngajati;
    char localitate[20];



    NodABC* radacina = NULL;


    FILE* file = fopen("farmacii.txt", "r");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului.\n");
        return -1;
    }

    while (fscanf(file, "%hu %s %f %s %hhu %s", &idFarmacie, denumireFarmacie, &cifraAfaceriAnuala, numeFarmacistSef, &nrAngajati, localitate) == 6) {
        Farmacie* farmacie = initializareFarmacie(idFarmacie, denumireFarmacie, cifraAfaceriAnuala, numeFarmacistSef, nrAngajati, localitate);
        radacina = inserarearbore(radacina, farmacie);
    }

    fclose(file);

    printf("Afisare preordine:\n");
    afisarepreordine(radacina);

    printf("\nAfisare inordine:\n");
    afisareinordine(radacina);

    printf("\nAfisare postordine:\n");
    afisarepostordine(radacina);

    dezalocarearbore(radacina);

    return 0;
}