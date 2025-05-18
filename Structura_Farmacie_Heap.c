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

typedef struct NodABC
{
    Farmacie* info;
    struct NodABC* left;
    struct NodABC* right;
}NodABC;

typedef struct MaxHeap {
    Farmacie** vector;
    int dimensiune;

}MaxHeap;

Farmacie* initializareFarmacie(unsigned short int idFarmacie, char* denumireFarmacie, float cifraAfaceriAnuala,
    char* numeFarmacistSef, unsigned char nrAngajati, char* localitate)
{
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

NodABC* initializareNod(Farmacie* farmacie)
{
    NodABC* nodNou = (NodABC*)malloc(sizeof(NodABC));
    nodNou->info = farmacie;
    nodNou->left = NULL;
    nodNou->right = NULL;

    return nodNou;
}

//AVL
int inaltime(NodABC* radacina)
{
    if (radacina == NULL)
    {
        return 0; // pt ca noi intoarcem int in functie
    }
    else
    {
        int hSubarboreStang = inaltime(radacina->left);
        int hSubarboreDrept = inaltime(radacina->right);

        if (hSubarboreStang > hSubarboreDrept)
        {
            return hSubarboreStang + 1;
        }
        else
        {
            return hSubarboreDrept + 1;
        }
    }
}

int gradEchilibru(NodABC* radacina)
{
    if (radacina == NULL)
    {
        return 0;
    }
    return inaltime(radacina->left) - inaltime(radacina->right);
}
//mereu avem NodABC* atunci cand modificam cumva structura arborelui
NodABC* rotireDreapta(NodABC* radacina)
{
    NodABC* x = radacina->left;
    NodABC* y = x->right;
    x->right = radacina;
    radacina->left = y;

    return x;
}
NodABC* rotireStanga(NodABC* radacina)
{
    NodABC* x = radacina->right;
    NodABC* y = x->left;
    x->left = radacina;
    radacina->right = y;

    return x;
}

NodABC* inserareAVL(NodABC* radacina, Farmacie* farmacie)
{
    if (radacina == NULL)
    {
        return initializareNod(farmacie);//creeaza un nod nou in spate
    }
    if (farmacie->idFarmacie < radacina->info->idFarmacie)
    {
        radacina->left = inserareAVL(radacina->left, farmacie);
    }
    if (farmacie->idFarmacie > radacina->info->idFarmacie)
    {
        radacina->right = inserareAVL(radacina->right, farmacie);
    }
    else
    {
        return radacina;
    }

    int balance = gradEchilibru(radacina);

    //farmacie->idFarmacie  =>  id-ul noii farmacii
    if (balance > 1 && farmacie->idFarmacie < radacina->left->info->idFarmacie)
    {
        return rotireDreapta(radacina);
    }
    if (balance<-1 && farmacie->idFarmacie>radacina->right->info->idFarmacie)
    {
        return rotireStanga(radacina);
    }

    if (balance > 1 && farmacie->idFarmacie > radacina->left->info->idFarmacie)
    {
        radacina->left = rotireStanga(radacina->left);
        return rotireDreapta(radacina);
    }
    if (balance < -1 && farmacie->idFarmacie < radacina->right->info->idFarmacie)
    {
        radacina->right = rotireDreapta(radacina->right);
        return rotireStanga(radacina);
    }

    return radacina;
}

NodABC* inserareInArbore(NodABC* radacina, Farmacie* farmacie)
{
    if (radacina == NULL)
    {
        return initializareNod(farmacie);//creeaza un nod nou in spate
    }
    if (farmacie->idFarmacie < radacina->info->idFarmacie)
    {
        radacina->left = inserareInArbore(radacina->left, farmacie);
    }
    if (farmacie->idFarmacie > radacina->info->idFarmacie)
    {
        radacina->right = inserareInArbore(radacina->right, farmacie);
    }

    return radacina;
}

void afisarePreordine(NodABC* radacina)
{
    if (radacina != NULL)
    {
        printf("%hu %s %5.2f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef, radacina->info->nrAngajati,
            radacina->info->localitate);

        afisarePreordine(radacina->left);
        afisarePreordine(radacina->right);
    }
}
void afisareInordine(NodABC* radacina)
{
    if (radacina != NULL)
    {
        afisareInordine(radacina->left);

        printf("%hu %s %5.2f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef, radacina->info->nrAngajati,
            radacina->info->localitate);

        afisareInordine(radacina->right);
    }
}
void afisarePostordine(NodABC* radacina)
{
    if (radacina != NULL)
    {
        afisarePostordine(radacina->left);

        afisarePostordine(radacina->right);

        printf("%hu %s %5.2f %s %hhu %s\n", radacina->info->idFarmacie, radacina->info->denumireFarmacie,
            radacina->info->cifraAfaceriAnuala, radacina->info->numeFarmacistSef, radacina->info->nrAngajati,
            radacina->info->localitate);
    }
}

void dezalocareArbore(NodABC* radacina)
{
    if (radacina != NULL)
    {
        dezalocareArbore(radacina->left);
        dezalocareArbore(radacina->right);

        free(radacina->info->denumireFarmacie);
        free(radacina->info->numeFarmacistSef);
        free(radacina->info->localitate);
        free(radacina->info);
        free(radacina);
    }
}

//ex2
int nrTotalAngajatiInLocalitateaX(NodABC* radacina, char* localitate)
{
    int nrAngatati = 0;
    if (radacina != NULL)
    {
        if (strcmp(radacina->info->localitate, localitate) == 0)
        {
            nrAngatati += radacina->info->nrAngajati;
        }
        nrAngatati += nrTotalAngajatiInLocalitateaX(radacina->left, localitate);
        nrAngatati += nrTotalAngajatiInLocalitateaX(radacina->right, localitate);
    }
    return nrAngatati;
}

//ex3
float cifraAfaceriMedieLunara(NodABC* radacina, unsigned short int idFarmacie)
{
    if (radacina == NULL)
    {
        return 0;
    }
    if (radacina->info->idFarmacie == idFarmacie)
    {
        return radacina->info->cifraAfaceriAnuala / 12;
    }
    float cifraAfaceriSubarboreStang = cifraAfaceriMedieLunara(radacina->left, idFarmacie);
    if (cifraAfaceriSubarboreStang != 0)
        return cifraAfaceriSubarboreStang;
    return cifraAfaceriMedieLunara(radacina->right, idFarmacie);
}

//ex4
//Nod frunza => left si right sunt 0
NodABC* stergereFrunzePeNivel(NodABC* radacina, int nivelCurent, int nivelSpecificat)
{
    if (radacina == NULL)
    {
        return NULL;
    }
    if (nivelCurent == nivelSpecificat)
    {
        if (radacina->left == NULL && radacina->right == NULL)
        {
            free(radacina->info->denumireFarmacie);
            free(radacina->info->numeFarmacistSef);
            free(radacina->info->localitate);
            free(radacina->info);
            free(radacina);

            return NULL;
        }
    }
    else
    {
        radacina->left = stergereFrunzePeNivel(radacina->left, nivelCurent + 1, nivelSpecificat);
        radacina->right = stergereFrunzePeNivel(radacina->right, nivelCurent + 1, nivelSpecificat);
    }
    return radacina;
}

//stergere radacina
NodABC* minim(NodABC* radacina)
{
    //cautam minimul, deci mergem pe left
    while (radacina != NULL && radacina->left != NULL)
    {
        radacina = radacina->left;
    }
    return radacina;
}

NodABC* stergereRadacina(NodABC* radacina)
{
    NodABC* temp;
    if (radacina == NULL)
    {
        return NULL;
    }
    if (radacina->left == NULL)
    {
        temp = radacina->right;
    }
    else if (radacina->right == NULL)
    {
        temp = radacina->left;
    }
    else
    {

        NodABC* nouaRadacina = minim(radacina->right);
        radacina->info->idFarmacie = nouaRadacina->info->idFarmacie;

        free(radacina->info->denumireFarmacie);
        radacina->info->denumireFarmacie = (char*)malloc(strlen(nouaRadacina->info->denumireFarmacie) + 1);
        strcpy(radacina->info->denumireFarmacie, nouaRadacina->info->denumireFarmacie);

        radacina->info->cifraAfaceriAnuala = nouaRadacina->info->cifraAfaceriAnuala;

        free(radacina->info->numeFarmacistSef);
        radacina->info->numeFarmacistSef = (char*)malloc(strlen(nouaRadacina->info->numeFarmacistSef) + 1);
        strcpy(radacina->info->numeFarmacistSef, nouaRadacina->info->numeFarmacistSef);

        radacina->info->nrAngajati = nouaRadacina->info->nrAngajati;

        free(radacina->info->localitate);
        radacina->info->localitate = (char*)malloc(strlen(nouaRadacina->info->localitate) + 1);
        strcpy(radacina->info->localitate, nouaRadacina->info->localitate);

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

//int e cu pointer* pt ca mereu se modifica, se apeleaza recursiv
void inserareHeap(Farmacie** vector, int* dimensiune, NodABC* radacina) {
    if (radacina != NULL) {
        vector[*dimensiune] = radacina->info;
        (*dimensiune)++;
        inserareHeap(vector, dimensiune, radacina->left);
        inserareHeap(vector, dimensiune, radacina->right);
    }

}


//HEAPFY e denumirea standard
//aici e fara pointer la dimensiune
//alegem noi un camp dupa care sa facem comparatia: int, float, double etc
void filtrareHeap(Farmacie** vector, int dimensiune, int index) {
    int max = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    //daca aveam minHeap, aveam semnul de mai mare peste tot in loc de mai mic

    if (left < dimensiune && vector[left]->cifraAfaceriAnuala < vector[max]->cifraAfaceriAnuala) {
        max = left;
    }

    if (right < dimensiune && vector[right]->cifraAfaceriAnuala < vector[max]->cifraAfaceriAnuala) {
        max = right;
    }

    if (max != index) {
        Farmacie* temp = vector[index];
        vector[index] = vector[max];
        vector[max] = temp;
        filtrareHeap(vector, dimensiune, max);
    }


}

MaxHeap conversieArboreHeap(NodABC* radacina, int nrNoduri) {
    MaxHeap maxHeap;
    // e punct ca nu e declarat ca pointer
    maxHeap.vector = (Farmacie**)malloc(nrNoduri * sizeof(Farmacie*));
    maxHeap.dimensiune = 0;
    inserareHeap(maxHeap.vector, &maxHeap.dimensiune, radacina);

    //se insereaza de la jumatate

    for (int i = maxHeap.dimensiune / 2 - 1; i >= 0; i--) {
        filtrareHeap(maxHeap.vector, maxHeap.dimensiune, i);
    }

    return maxHeap;
}

//functie care determina nr Noduri din arbore => din stang si drept adunare
int nrNoduriArbore(NodABC* radacina) {
    if (radacina == NULL) {
        return 0;
    }
    return 1 + nrNoduriArbore(radacina->left) + nrNoduriArbore(radacina->right);
}

//puteam si pointer si foloseam -> in loc de .
void afisareHeap(MaxHeap maxHeap) {
    for (int i = 0; i < maxHeap.dimensiune; i++) {
        printf("%hu %s %5.2f %s %hhu %s\n", maxHeap.vector[i]->idFarmacie, maxHeap.vector[i]->denumireFarmacie, maxHeap.vector[i]->cifraAfaceriAnuala, maxHeap.vector[i]->numeFarmacistSef, maxHeap.vector[i]->nrAngajati, maxHeap.vector[i]->localitate);
    }
}

int nrNoduriPeNivele(NodABC* radacina, int nivel) {
    if (radacina == NULL) {
        return 0;
    }
    if (nivel == 0) {
        return 1;
    }
    return nrNoduriPeNivele(radacina->left, nivel - 1) + nrNoduriPeNivele(radacina->right, nivel - 1);
}


void afisareNrNoduriNivel(NodABC* radacina) {
    int h = inaltime(radacina);
    for (int i = 0; i < h; i++) {
        printf("Nivel %d: %d noduri\n", i, nrNoduriPeNivele(radacina, i));
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


    FILE* file = fopen("fisier.txt", "r");
    while (fscanf(file, "%hu %s %f %s %hhu %s", &idFarmacie, denumireFarmacie, &cifraAfaceriAnuala, numeFarmacistSef, &nrAngajati, localitate) == 6) {
        Farmacie* farmacie = initializareFarmacie(idFarmacie, denumireFarmacie, cifraAfaceriAnuala, numeFarmacistSef, nrAngajati, localitate);
        radacina = inserareAVL(radacina, farmacie);
    }

    afisarePreordine(radacina);
    fclose(file);

    //ex2
    int nrTotalAngajati = nrTotalAngajatiInLocalitateaX(radacina, "Bucuresti");
    printf("\n%d\n", nrTotalAngajati);

    //ex3
    float cifraAfacereMedie = cifraAfaceriMedieLunara(radacina, 5);
    printf("%5.2f\n", cifraAfacereMedie);

    //ex4

    radacina = stergereFrunzePeNivel(radacina, 0, 2);
    afisareInordine(radacina);

    printf("\n");

    radacina = stergereRadacina(radacina);
    afisareInordine(radacina);

    printf("--------------------------------------\n");

    int nrNoduri = nrNoduriArbore(radacina);
    printf("%d\n", nrNoduri);

    MaxHeap maxHeap = conversieArboreHeap(radacina, nrNoduri);
    afisareHeap(maxHeap);
    printf("--------------------------------------\n");

    printf("\nNumarul de noduri pe fiecare nivel:\n");
    afisareNrNoduriNivel(radacina);


    dezalocareArbore(radacina);


}