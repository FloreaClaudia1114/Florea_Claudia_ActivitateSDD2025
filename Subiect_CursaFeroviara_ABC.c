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