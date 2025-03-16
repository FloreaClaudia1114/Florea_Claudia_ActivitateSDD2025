#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct Comanda {
	unsigned int idComanda;
	char* statusComanda;
	char* numeClient;
	unsigned int nrProduse;
	float sumaPlata;
}Comanda;

typedef struct Nod {
	Comanda* info;
	struct Nod* next;
	struct Nod* prev;

}Nod;
Comanda* initializareComanda(unsigned int idComanda, char* statusComanda, char* numeClient, unsigned int nrProduse, float sumaPlata) {
	Comanda* c = (Comanda*)malloc(sizeof(Comanda));
	c->idComanda = idComanda;
	c->statusComanda = (char*)malloc(sizeof(statusComanda) + 1);
	strcpy(c->statusComanda, statusComanda);
	c->numeClient = (char*)malloc(sizeof(numeClient) + 1);
	strcpy(c->numeClient, numeClient);
	c->nrProduse = nrProduse;
	c->sumaPlata = sumaPlata;
	return c;
};
Nod* initializareNod(Comanda* c) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = c;
	nod->next = NULL;
	nod->prev = NULL;

	return nod;
}
Nod* inserareAlternativ(Nod* cap, Nod** coada, Comanda* c, int* alternanta) {
	Nod* nodNou = initializare(c);
	if (cap == NULL) {
		cap = nodNou;
		*coada = nodNou;
	}
	else
		if (*alternanta % 2 == 0) {
			nodNou->next = cap;
			cap->prev = nodNou;

		}
		else {
			(*coada)->next = nodNou;
			nodNou->prev = coada;
			*coada = nodNou;


		}
	(*alternanta)++;
	return cap;

}



