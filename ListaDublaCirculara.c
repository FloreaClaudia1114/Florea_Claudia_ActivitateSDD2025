#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct Student {

	unsigned int id;
	char* nume;
	unsigned char nrMaterii; 
	int* note;

}Student;

typedef struct nod {

	Student info;
	struct nod* next;
	struct nod* prev;

}nod;

typedef struct LD {

	nod* prim;
	nod* ultim;

}LD;

Student initializareStudent(int id, const char* nume, unsigned char nrMaterii, int* note) {

	Student s;
	s.id = id;
	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy_s(s.nume, sizeof(char) * (strlen(nume) + 1), nume);


	s.nrMaterii = nrMaterii;
	
	s.note = (int*)malloc(sizeof(int) * nrMaterii);
	
	for (int i = 0; i < nrMaterii; i++) {

		s.note[i] = note[i];

	}

	return s;

}

LD inserare(LD lista, Student s) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->prev = NULL;
	nou->next = NULL;
	nou->info = initializareStudent(s.id, s.nume, s.nrMaterii, s.note);

	if (lista.prim == NULL) {

		lista.prim = lista.ultim = nou;
		nou->next = lista.prim;
		nou->prev = lista.prim;

	}
	else {

		lista.ultim->next = nou;
		nou->prev = lista.ultim;
		lista.prim->prev = nou;
		nou->next = lista.prim;
		lista.ultim = nou;

	}

	return lista;

}

void afisare(LD lista) {

	nod* aux = lista.prim;
	while (aux->next != lista.prim) {

		printf("\n %u, %s, %u \n", aux->info.id, aux->info.nume, aux->info.nrMaterii);
		for (int i = 0; i < aux->info.nrMaterii; i++) {

			printf("%d", aux->info.note[i]);

		}
		aux = aux->next;

	}

	printf("\n %u, %s, %u \n", aux->info.id, aux->info.nume, aux->info.nrMaterii);
	for (int i = 0; i < aux->info.nrMaterii; i++) {

		printf("%d", aux->info.note[i]);

	}


}

void dezalocare(LD lista) {

	nod* aux = lista.prim;
	while (aux->next != lista.prim) {

		free(aux->info.nume);
		free(aux->info.note);
		nod* temp = aux->next;
		free(aux);
		aux = temp;

	}

}

void main() {

	int nr;
	Student s;
	char buffer[50];
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	FILE* f = fopen("fisier.txt", "r");
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		fscanf(f, "%u", &s.id);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy_s(s.nume, sizeof(char) * (strlen(buffer) + 1), buffer);

		fscanf(f, "%u", &s.nrMaterii);
		s.note = (int*)malloc(sizeof(int) * s.nrMaterii);
		for (int i = 0; i < s.nrMaterii; i++) {

			fscanf(f, "%d", s.note[i]);

		}
		lista = inserare(lista, s);
		free(s.nume);
		free(s.note);
	}

	afisare(lista);
	dezalocare(lista);

}