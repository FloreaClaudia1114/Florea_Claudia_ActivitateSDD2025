#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Animal {

	unsigned int id;
	char* nume;
	int nrSpecimene;

}Animal;

typedef struct nod {

	Animal info;
	struct nod* next;

}nod;

Animal initializareAnimal(int id, const char* nume, int nrSpecimene) {

	Animal a;
	a.id = id;
	a.nume = (char*)malloc(sizeof(char) * strlen(nume) + 1);
	strcpy_s(a.nume, sizeof(char) * strlen(nume) + 1, nume);

	return a;

}

nod* inserare(nod* cap, Animal a) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = initializareAnimal(a.id, a.nume, a.nrSpecimene);
	nou->next = NULL;

	if (cap == NULL) {

		cap = nou;
		nou->next = cap;

	}
	else {

		nod* aux = cap;
		while (aux->next != cap) {

			aux = aux->next;

		}

		aux->next = nou;
		aux->next = cap;

	}

	return cap;

}

void afisare(nod* cap) {

	nod* aux = cap;
	while (aux->next != cap) {

		printf("%u %s %d \n", aux->info.id, aux->info.nume, aux->info.nrSpecimene);
		aux = aux->next;

	}

	printf("%u %s %d \n", aux->info.id, aux->info.nume, aux->info.nrSpecimene);

}

void dezalocare(nod* cap) {

	nod* aux = cap;
	while (aux->next != cap) {

		free(aux->info.nume);
		nod* temp = aux->next;
		free(aux);
		aux = temp;

	}

	free(aux->info.nume);
	free(aux);

}

void main() {

	int nr;
	Animal a;
	char buffer[50];
	nod* cap = NULL;

	FILE* f = fopen("animal.txt", "r");
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++) {

		fscanf(f, "%u", &a.id);
		fscanf(f, "%s", buffer);
		a.nume = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
		strcpy_s(a.nume, sizeof(char) * strlen(buffer) + 1, buffer);

		cap = inserare(cap, a);
		free(a.nume);

	}

	afisare(cap);

}