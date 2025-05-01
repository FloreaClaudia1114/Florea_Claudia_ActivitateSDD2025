#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

// Creare structura Proiect
typedef struct Proiect {

	unsigned int id;
	char* titluP;
	char* beneficiar;
	unsigned char nrExecutanti;
	float bugetA;

}Proiect;

// Functia de initializare
Proiect initializareProiect(unsigned int id, char* titluP, char* beneficiar,
	unsigned char nrExecutanti, float bugetA) {

	Proiect p;
	p.id = id;
	p.titluP = (char*)malloc(sizeof(char) * strlen(titluP) + 1);
	strcpy(p.titluP, titluP);
	p.beneficiar = (char*)malloc(sizeof(char) * strlen(beneficiar) + 1);
	strcpy(p.beneficiar, beneficiar);
	p.nrExecutanti = nrExecutanti;
	p.bugetA = bugetA;

	return p;

}

typedef struct Nod {

	Proiect proiect;
	struct Nod* next;
	struct Nod* prev;

}Nod;

// Citire din fisier
Proiect citireFisier(FILE* f) {

	char buffer[100];
	Proiect p = { 0, NULL, NULL, 0, 0.0f };
	if (fgets(buffer, sizeof(buffer), f)) {

		char* token = strtok(buffer, ",;\n");
		if (token == NULL) {

			return p;

		}
		p.id = atoi(token);

		token = strtok(NULL, ",;\n");
		if (token == NULL) {

			return p;

		}
		p.titluP = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(p.titluP, token);

		token = strtok(NULL, ",;\n");
		if (token == NULL) {

			return p;

		}
		p.beneficiar = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(p.beneficiar, token);

		token = strtok(NULL, ",;\n");
		if (token == NULL) {

			return p;

		}
		p.nrExecutanti = atoi(token);

		token = strtok(NULL, ",;\n");
		if (token == NULL) {

			return p;

		}
		p.bugetA = (float)atof(token);

	}

	return p;

}

// Functie de afisare a Proiectului
void afisareProiect(Proiect proiect) {

	printf("Id: %d\n", proiect.id);
	printf("Titilu proiect: %s\n", proiect.titluP);
	printf("Beneficiar: %s\n", proiect.beneficiar);
	printf("Nr executanti: %d\n", proiect.nrExecutanti);
	printf("Buget: %.2f\n", proiect.bugetA);

}

// Creare strcutura lista dubla
typedef struct ListaDubla {

	Nod* first;
	Nod* last;
	int nrNoduri;

}ListaDubla;

// Functia de afisare a listei de la inceput
void afisareListaProiecteInceput(ListaDubla lista) {

	Nod* p = lista.first;
	while (p) {

		afisareProiect(p->proiect);
		p = p->next;

	}

}

// Functia de afisare a listei de la sfarsit 
void afisareListaSfarsit(ListaDubla lista) {

	Nod* p = lista.last;
	while (p) {

		afisareProiect(p->proiect);
		p = p->prev;

	}

}

// Functia de adaugare a unui element la finalul listei
void adaugaLaFinalInLista(ListaDubla* lista, Proiect proiectNou) {

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->proiect = proiectNou;
	nou->next = NULL;
	nou->prev = lista->last;

	if (lista->last != NULL) {

		lista->last->next = nou;

	}
	else {

		lista->first = nou;

	}

	lista->last = nou;
	lista->nrNoduri++;

}

// Functia de adaugare a unui element la inceputul listei
void adaugaLaInceputInLista(ListaDubla* lista, Proiect proiectNou) {

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->proiect = proiectNou;
	nou->next = lista->first;
	nou->prev = NULL;

	if (lista->first != NULL) {

		lista->first->prev = nou;

	}
	else {

		lista->last = nou;

	}

	lista->first = nou;
	lista->nrNoduri++;

}

// Functia de citire a eleemntelor din fisier
ListaDubla citireLDDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;

	while (!feof(f)) {

		adaugaLaFinalInLista(&lista, citireFisier(f));

	}

	fclose(f);
	return lista;

}

// Functia care determina fondul de investitii
float fonduriBeneficiar(ListaDubla lista,
	const char* beneficiar) {

	float suma = 0.0f;
	Nod* p = lista.first;

	while (p) {

		if (strcmp(p->proiect.beneficiar, beneficiar) == 0) {

			suma += p->proiect.bugetA;

		}

		p = p->next;

	}

	return suma;
}

// Functia care modifca bugetul alocat
void modificaBugetProiecte(ListaDubla* lista,
	unsigned int* vectorIDuri, int nrProiecte,
	float procent) {

	for (int i = 0; i < nrProiecte; i++) {

		Nod* p = lista->first;
		while (p) {

			if (p->proiect.id == vectorIDuri[i]) {

				p->proiect.bugetA += p->proiect.bugetA * procent;

			}

			p = p->next;

		}

	}

}

// Functia de dezalocare a listei dublu inalntuite
void dezalocareLista(ListaDubla* lista) {

	Nod* p = lista->first;
	while (p) {

		Nod* aux = p;
		p = p->next;
		if (aux->proiect.titluP != NULL) {

			free(aux->proiect.titluP);

		}

		if (aux->proiect.beneficiar != NULL) {

			free(aux->proiect.beneficiar);

		}

		free(aux);

	}

	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;

}

// Afisare in main
int main() {

	ListaDubla lista = citireLDDinFisier("proiect.txt");
	afisareListaProiecteInceput(lista);

	printf("\n ============ Afisare lista de la sfarsit =========== \n");
	afisareListaSfarsit(lista);

	// Fonduri pentru beneficiar
	char numeBeneficiar[100];
	printf("\nIntroduceti numele beneficiarului pentru care doriti sa aflati fondurile: ");
	scanf("%s", numeBeneficiar);

	float totalFonduri = fonduriBeneficiar(lista, numeBeneficiar);
	printf("Fondurile totale pentru beneficiarul '%s' sunt: %.2f\n", numeBeneficiar, totalFonduri);

	// Modificare buget
	int nrIDuri = 0;
	printf("\nIntroduceti numarul de proiecte pentru modificare buget: ");
	scanf("%d", &nrIDuri);

	unsigned int* iduri = (unsigned int*)malloc(sizeof(unsigned int) * nrIDuri);
	for (int i = 0; i < nrIDuri; i++) {
		printf("ID proiect %d: ", i + 1);
		scanf("%u", &iduri[i]);
	}

	float procent;
	printf("Introduceti procentul de modificare (ex: 0.1 pentru +10%% sau -0.1 pentru -10%%): ");
	scanf("%f", &procent);

	modificaBugetProiecte(&lista, iduri, nrIDuri, procent);

	printf("\n===== Lista dupa modificarea bugetelor =====\n");
	afisareListaProiecteInceput(lista);

	free(iduri);

	dezalocareLista(&lista);
	printf("\n ============ Afisare lista dupa dezalocare =========== \n");
	afisareListaProiecteInceput(lista);

	return 0;

}