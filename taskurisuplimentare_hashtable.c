#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

typedef struct Cladire {
	int id;
	int an_construire;
	char* denumire;
	unsigned char categorie_risc;
} Cladire;

typedef struct Nod {
	Cladire cladire;
	Nod* next;
} Nod;

//creare structura tabela HT, aceasta contine un vector de pointeri la nod care sa retina nodurile si aceasta dimensiune
typedef struct HT {
	int dim;
	Nod** tabela;

}HT;
Cladire citirecladiredinfisier(FILE* file) {
	Cladire c;
	char buffer[100];
	char sep[4] = ".;\n";
	fgets(buffer, 100, file);
	c.id = atoi(strtok(buffer, sep));
	c.an_construire = atoi(strtok(NULL, sep));

	char* aux;
	aux = strtok(NULL, sep);
	c.denumire = (char*)malloc(strlen(aux) + 1);//alocam memorie
	strcpy(c.denumire, aux);//destinatia si sursa

	c.categorie_risc = strtok(NULL, sep)[0];//il indexam luam un singur element
	return c;

}

//functia de afisare a unei cladire  
//%c - un singur caracter, %s un sir de caractere pt pointer
void afisarecladire(Cladire cladire) {
	printf("Id: %d\n", cladire.id);
	printf("An_construire: %d\n", cladire.an_construire);
	printf("Denumire: %s\n", cladire.denumire);
	printf("Categorie_risc: %c\n", cladire.categorie_risc);
}

void afisarelistacladiri(Nod* cap) {
	while (cap) {
		afisarecladire(cap->cladire);
		cap = cap->next;
	}
}

void cladirenouainlista(Nod* cap, Cladire cladireNoua) {
	Nod* p = cap;//ne deplasam pn la ultimul nod 
	while (p->next) {
		p = p->next;
	}
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->cladire = cladireNoua;//shallow copy
	nou->next = NULL;
	p->next = nou;
}

HT initializareHT(int dimensiune) {

}