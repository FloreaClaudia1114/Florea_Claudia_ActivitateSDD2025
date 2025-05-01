#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct Student {

	unsigned int id;
	char* nume;

}Student;

typedef struct nod {

	Student info;
	struct nod* next;

}nod;

Student initializareStudent(int id, const char* nume) {

	Student s;
	s.id = id;
	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy_s(s.nume, sizeof(char) * (strlen(nume) + 1), nume);

	return s;

}

nod* inserare(nod* cap, Student s) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = initializareStudent(s.id, s.nume);
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
		nou->next = cap;

	}

	return cap;

}

void afisare(nod* cap) {

	nod* aux = cap;
	while (aux->next != cap) {

		printf("%u, %s \n", aux->info.id, aux->info.nume);
		aux = aux->next;

	}

	printf("%u, %s \n", aux->info.id, aux->info.nume);

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
	Student s;
	char buffer[50];
	nod* cap = NULL;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++) {

		fscanf(f, "%u", &s.id);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy_s(s.nume, sizeof(char) * (strlen(buffer) + 1), buffer);

		cap = inserare(cap, s);
		free(s.nume);

	}

	afisare(cap);

}