﻿#define _CRT_SECURE_NO_WARNINGS
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

void dezalocareArbore(NodABC* radacina) {

	if (radacina != NULL) {
		dezalocareArbore(radacina->left);
		dezalocareArbore(radacina->right);
		free(radacina->info->datacursa);
		free(radacina->info);
		free(radacina);
	}

}
//functia de stergere a unui nod pentru care se primeste id-ul de cautare
NodABC* stergereNodArbore(NodABC* radacina, unsigned int id) {
	if (radacina == NULL) {
		return NULL;
	}

	if (id < radacina->info->nrcursa) {
		radacina->left = stergereNodArbore(radacina->left, id);
	}
	else if (id > radacina->info->nrcursa) {
		radacina->right = stergereNodArbore(radacina->right, id);
	}
	else {
		// Nod gasit
		if (radacina->left == NULL) {
			NodABC* temp = radacina->right;
			free(radacina->info->datacursa);
			free(radacina->info);
			free(radacina);
			return temp;
		}
		else if (radacina->right == NULL) {
			NodABC* temp = radacina->left;
			free(radacina->info->datacursa);
			free(radacina->info);
			free(radacina);
			return temp;
		}

		// Nod cu doi copii
		NodABC* temp = radacina->right;
		while (temp->left != NULL) {
			temp = temp->left;
		}

		// Copierea valorilor succesorului in nodul curent
		radacina->info->nrcursa = temp->info->nrcursa;
		free(radacina->info->datacursa);  // Eliberare memorie pentru vechiul șir
		radacina->info->datacursa = (char*)malloc(strlen(temp->info->datacursa) + 1);  // Alocare memorie pentru noul șir
		if (radacina->info->datacursa != NULL) {
			strcpy(radacina->info->datacursa, temp->info->datacursa);  // Copierea șirului
		}
		radacina->info->nrvagoane = temp->info->nrvagoane;
		radacina->info->nrbilete = temp->info->nrbilete;
		radacina->info->pretbilet = temp->info->pretbilet;


		// Stergerea succesorului
		radacina->right = stergereNodArbore(radacina->right, temp->info->nrcursa);
	}

	return radacina;
}

// Functie care calculeaza inaltimea unui subarbore
int inaltimeArbore(NodABC* radacina) {
	if (radacina == NULL) {
		return 0; // Inaltimea unui arbore gol este 0
	}

	int inaltimeStanga = inaltimeArbore(radacina->left);
	int inaltimeDreapta = inaltimeArbore(radacina->right);

	return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta);
}

// Functie care returneaza subarborele cu inaltimea mai mare
NodABC* subarboreMaiInalt(NodABC* radacina) {
	if (radacina == NULL) {
		return NULL;
	}


	int inaltimeStanga = inaltimeArbore(radacina->left);
	int inaltimeDreapta = inaltimeArbore(radacina->right);


	if (inaltimeStanga >= inaltimeDreapta) {
		return radacina->left;
	}
	else {
		return radacina->right;
	}
}


// Functie care calculeaza numarul de noduri din subordine pentru un nod radacina
int numarNoduriSubordine(NodABC* radacina) {
	if (radacina == NULL) {
		return 0;
	}


	int noduriStanga = numarNoduriSubordine(radacina->left);
	int noduriDreapta = numarNoduriSubordine(radacina->right);


	return 1 + noduriStanga + noduriDreapta;
}

// Functie care returneaza subarborele cu mai multe noduri in subordine
NodABC* subarboreMaiMultiNoduri(NodABC* radacina) {
	if (radacina == NULL) {
		return NULL;
	}

	if (radacina->left == NULL && radacina->right == NULL) {
		printf("Nodul nu are subarbore.\n");
		return NULL;
	}

	// Calculam numarul de noduri din subordine pentru fiecare subarbore
	int noduriStanga = numarNoduriSubordine(radacina->left);
	int noduriDreapta = numarNoduriSubordine(radacina->right);

	if (noduriStanga >= noduriDreapta) {
		return radacina->left;
	}
	else {
		return radacina->right;
	}
}


int main() {
	unsigned int nrcursa;
	char datacursa[100];
	unsigned char nrvagoane;
	unsigned short int nrbilete;
	float pretbilet;

	NodABC* radacina = NULL;


	FILE* file = fopen("curse.txt", "r");
	if (!file) {
		perror("Eroare la deschiderea fisierului:");
		return 1;
	}


	printf("Fisier deschis cu succes\n");

	while (fscanf(file, "%u %s %hhu %hu %f", &nrcursa, datacursa, &nrvagoane, &nrbilete, &pretbilet) == 5) {
		printf("Citit cursa: %u %s %hhu %hu %.2f\n", nrcursa, datacursa, nrvagoane, nrbilete, pretbilet);

		Cursa* cursa = initializareCursa(nrcursa, datacursa, nrvagoane, nrbilete, pretbilet);
		radacina = inserareArbore(radacina, cursa);
	}



	fclose(file);

	printf("Afisare preordine inainte de stergere nod\n");
	afisarePreordine(radacina);



	unsigned int idDeSters;
	printf("\nIntroduceti ID-ul cursei de sters: ");
	scanf("%u", &idDeSters);

	radacina = stergereNodArbore(radacina, idDeSters);

	printf("\nAfisare preordine dupa stergere:\n");
	afisarePreordine(radacina);

	//Apelam functia pentru a obtine subarborele cu inaltimea mai mare
	NodABC* subarbore = subarboreMaiInalt(radacina);


	if (subarbore != NULL) {
		printf("Subarborele cu inaltimea mai mare are radacina: %u\n", subarbore->info->nrcursa);
	}
	else {
		printf("Nu exista subarbore.\n");
	}

	// Verificam numarul de noduri din subordine pentru radacina
	printf("\nNumarul de noduri aflate in subordine pentru radacina este: %d\n", numarNoduriSubordine(radacina) - 1);

	// Apelam functia pentru a obtine subarborele cu mai multe noduri
	NodABC* subarboreMaxNoduri = subarboreMaiMultiNoduri(radacina);
	if (subarboreMaxNoduri != NULL) {
		printf("\nSubarborele cu mai multe noduri in subordine are radacina: %u\n", subarboreMaxNoduri->info->nrcursa);
	}
	else {
		printf("\nNodul nu are subarbore cu noduri.\n");
	}

	dezalocareArbore(radacina);
	return 0;
}








