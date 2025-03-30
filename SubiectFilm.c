#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/// lista dubla
typedef struct Film {
	char* titlu;
	float rating;
	unsigned char nr_minute;
	char* gen;

}Film;

typedef struct Nod {
	Film* Info;
	struct Nod* next;
	struct Nod* prev;
}Nod;

Film* initializareFilm(char* titlu, float rating, unsigned char nr_minute, char* gen) {
	Film* film = (Film*)malloc(sizeof(Film));
	film->titlu = (char*)malloc(strlen(titlu) + 1);
	strcpy(film->titlu, titlu);
	film->rating = rating;
	film->nr_minute = nr_minute;
	film->gen = (char*)malloc(strlen(gen) + 1);
	strcpy(film->gen, gen);

	return film;

}
//
Nod* initializareNod(Film* film) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->Info = film;
	nod->next = NULL;
	nod->prev = NULL;

	return nod;
}
///inserare, daca nu ne zic, facem inserarea la sfarsit, daca se specifica: cap(la inceput), coada(la sfarsit), in lista dubla atata capul cat si coada vor fi primul elem adica noulnod(nodnou)

Nod* inserarelistadubla(Nod* cap, Nod** coada, Film* film) {
	Nod* nodnou = initializareNod(film);
	if (cap == NULL) {
		cap = nodnou;
		*coada = nodnou;
	}

	//inserarea la sfarsit
	else {
		(*coada)->next = nodnou;
		nodnou->prev = *coada;
		*coada = nodnou;

	}

	return cap;
	///coada e legata de cap, mereu returnam capul, e cel mai important, in functie de el incepem inserarea
}
//afisarea normala a listei duble
void afisarelistadubla(Nod* cap) {
	if (cap != NULL) {
		while (cap != NULL) {

			printf("%s %5.2f  %hhu %s\n", cap->Info->titlu, cap->Info->rating, cap->Info->nr_minute, cap->Info->gen);
			cap = cap->next;


		}
	}
}
//afisare inversa a listei duble
void afisareinversalistadubla(Nod* coada) {
	if (coada != NULL) {
		while (coada != NULL) {

			printf("%s %5.2f  %hhu %s\n", coada->Info->titlu, coada->Info->rating, coada->Info->nr_minute, coada->Info->gen);
			coada = coada->prev;


		}
	}
}

//dezalocarea

void dezalocare(Nod* cap) {
	if (cap != NULL) {
		while (cap != NULL) {
			Nod* aux = cap;
			cap = cap->next;
			free(aux->Info->titlu);
			free(aux->Info->gen);
			free(aux->Info);
			free(aux);

		}
	}
}
//exercitiu 2 valoarea este arbitrara, noi o dam, toate functiile se fac inainte de main

int nrfilmepesteprag(Nod* cap, unsigned char pragnr_minute) {
	int nrfilmepesteprag = 0;
	if (cap != NULL) {
		while (cap != NULL) {
			if (cap->Info->nr_minute > pragnr_minute) {
				nrfilmepesteprag++;
			}
			cap = cap->next;

		}
	}

	return nrfilmepesteprag;
}
///strcmp 

//exercitiu 4
int nrFilmeNume(Nod* cap, char* titlu) {
	int nrFilme2 = 0;
	while (cap != NULL) {
		if (strcmp(cap->Info->titlu, titlu) == 0) {
			nrFilme2++;
		}
		cap = cap->next;
	}
	return nrFilme2;

}
//media pe rating

float medieRating(Nod* cap) {
	int nrFilme3 = 0;
	float sumaTotalRating = 0;
	while (cap != NULL) {

		sumaTotalRating += cap->Info->rating;
		nrFilme3++;
		cap = cap->next;

	}
	return sumaTotalRating / nrFilme3;
}
//cream o lista dubla in care mutam, un cap nou, o coada noua
Nod* mutarefilmepestemedie(Nod* cap, Nod** coadanoua, float valoaremedie) {
	Nod* capnou = NULL;
	*coadanoua = NULL;
	while (cap != NULL) {
		if (cap->Info->rating > valoaremedie) {
			Film* film = initializareFilm(cap->Info->titlu, cap->Info->rating, cap->Info->nr_minute, cap->Info->gen);
			capnou = inserarelistadubla(capnou, coadanoua, film);
		}

		cap = cap->next;

	}

	return capnou;

}


///apelul, \n doar in afisare, //gen deoarece este vector
int main() {
	char titlu[40];
	float rating;
	unsigned char nr_minute;
	char gen[40];
	FILE* file = fopen("filme.txt", "r");
	Nod* cap = NULL;
	Nod* coada = NULL;
	while (fscanf(file, "%s %f %hhu %s", titlu, &rating, &nr_minute, gen) == 4) {
		Film* film = initializareFilm(titlu, rating, nr_minute, gen);
		cap = inserarelistadubla(cap, &coada, film);
	}

	fclose(file);
	afisarelistadubla(cap);

	printf("=======================================\n");

	afisareinversalistadubla(coada);
	printf("=======================================\n");
	//afisam functia, variabila sa aiba alt nume decat in functie , (pt int avem %d
	int nrfilmepeste = nrfilmepesteprag(cap, 140);
	printf("%d \n", nrfilmepeste);
	//afisam valoarea medie
	float nrFilme3 = 0.0;
	nrFilme3 = medieRating(cap);
	printf("Media dupa rating este %5.2f\n", nrFilme3);
	printf("-------------------------------------------------------------\n");

	//afisam la ex 4 
	Nod* capnou = NULL;
	Nod* coadanoua = NULL;
	capnou = mutarefilmepestemedie(cap, &coadanoua, nrFilme3);

	//afisare lista dubla de cap nou
	afisarelistadubla(capnou);
}


