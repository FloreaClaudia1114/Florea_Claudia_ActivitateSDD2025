#define _CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Cursa {
	unsigned int nrcursa;
	char* datacursa;//pointer
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
	NodABC* nodNou = (NodABC*)malloc(sizeof(nodNou) + 1);
	nodNou->info = cursa;
	nodNou->left = NULL;
	nodNou->right = NULL;

	return nodNou;
}
NodABC* inserareArbore(NodABC* radacina, Cursa* cursa) {
	if (radacina == NULL) {
		return initializareNod(cursa);
	}

	if (cursa->nrcursa < radacina->info->nrcursa) {
		radacina->left = inserareArbore(radacina->left, radacina);
	}
	else if (cursa->nrcursa > radacina->info->nrcursa) {
		radacina->right = inserareArbore(radacina->right, radacina);
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

}
