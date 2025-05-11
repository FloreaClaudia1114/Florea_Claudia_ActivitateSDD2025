#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Cursa {
	unsigned int nrcursa;
	char* datacursa;
	unsigned char nrvagoane;
	unsigned short int nrbilete;
	float pretbilet;
} Cursa;

typedef struct NodABC {
	Cursa* info;
	struct NodABC* left;
	struct NodABC* right;
} NodABC;

Cursa* initializareCursa(unsigned int nrcursa, char* datacursa, unsigned char nrvagoane, unsigned short int nrbilete, float pretbilet) {
	Cursa* cursa = (Cursa*)malloc(sizeof(Cursa));

	cursa->nrcursa = nrcursa;
	cursa->datacursa = (char*)malloc(strlen(datacursa) + 1);
	strcpy(cursa->datacursa, datacursa);

	cursa->nrvagoane = nrvagoane;
	cursa->nrbilete = nrbilete;
	cursa->pretbilet = pretbilet;

	return cursa;
}

NodABC* initializareNod(Cursa* cursa) {
	NodABC* nodNou = (NodABC*)malloc(sizeof(NodABC));
	nodNou->info = cursa;
	nodNou->left = NULL;
	nodNou->right = NULL;

	return nodNou;
}

NodABC* inserareArbore(NodABC* radacina, Cursa* cursa) {
	printf("Inserare in arbore: %u\n", cursa->nrcursa);

	if (radacina == NULL) {
		printf("Inserare cursa: %u\n", cursa->nrcursa);
		return initializareNod(cursa);
	}

	if (cursa->nrcursa < radacina->info->nrcursa) {
		radacina->left = inserareArbore(radacina->left, cursa);
	}
	else if (cursa->nrcursa > radacina->info->nrcursa) {
		radacina->right = inserareArbore(radacina->right, cursa);
	}

	return radacina;
}

void afisarePreordine(NodABC* radacina) {
	if (radacina != NULL) {
		printf("%u %s %hhu %hu %.2f\n",
			radacina->info->nrcursa,
			radacina->info->datacursa,
			radacina->info->nrvagoane,
			radacina->info->nrbilete,
			radacina->info->pretbilet);

		afisarePreordine(radacina->left);
		afisarePreordine(radacina->right);
	}
}

void afisareInordine(NodABC* radacina) {
	if (radacina != NULL) {
		afisareInordine(radacina->left);

		printf("%u %s %hhu %hu %.2f\n",
			radacina->info->nrcursa,
			radacina->info->datacursa,
			radacina->info->nrvagoane,
			radacina->info->nrbilete,
			radacina->info->pretbilet);

		afisareInordine(radacina->right);
	}
}

void afisareSubordine(NodABC* radacina) {
	if (radacina != NULL) {
		afisareInordine(radacina->left);
		afisareInordine(radacina->right);

		printf("%u %s %hhu %hu %.2f\n",
			radacina->info->nrcursa,
			radacina->info->datacursa,
			radacina->info->nrvagoane,
			radacina->info->nrbilete,
			radacina->info->pretbilet);


	}
}
int main() {
	unsigned int nrcursa;
	char datacursa[100];
	unsigned char nrvagoane;
	unsigned short int nrbilete;
	float pretbilet;

	NodABC* radacina = NULL;
	/// citim fisierul 

	FILE* file = fopen("curse.txt", "r");
	if (!file) {
		perror("Eroare la deschiderea fisierului:");
		return 1;
	}


	printf("Fisier deschis cu succes\n");

	while (fscanf(file, "%u %s %hhu %hu %.2f", &nrcursa, datacursa, &nrvagoane, &nrbilete, &pretbilet) == 5) {

		printf("Citit cursa: %u %s %hhu %hu %.2f\n", nrcursa, datacursa, nrvagoane, nrbilete, pretbilet);


		Cursa* cursa = initializareCursa(nrcursa, datacursa, nrvagoane, nrbilete, pretbilet);
		radacina = inserareArbore(radacina, cursa);
	}
	fclose(file);

	printf("Afisare preordine:\n");
	afisarePreordine(radacina);


}




