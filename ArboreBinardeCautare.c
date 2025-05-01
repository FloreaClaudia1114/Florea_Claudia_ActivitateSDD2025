#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct Rezervare rezervare;
typedef struct Nod nod;

struct Rezervare {

	unsigned int id;
	char* hotel;
	char* client;
	float suma;

};

rezervare createRezervare(unsigned int id, char* hotel,
	char* client, float suma) {

	rezervare r;
	r.id = id;
	r.hotel = (char*)malloc(sizeof(char) * strlen(hotel) + 1);
	strcpy(r.hotel, hotel);
	r.client = (char*)malloc(sizeof(char) * strlen(client) + 1);
	strcpy(r.client, hotel);
	r.suma = suma;

	return r;

}

struct Nod {

	rezervare info;
	nod* stanga;
	nod* dreapta;

};

nod* creareNod(rezervare r, nod* stanga, nod* dreapta) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = createRezervare(r.id, r.hotel, r.client, r.suma);
	nou->stanga = stanga;
	nou->dreapta = dreapta;

}

//Cheie de inserare = id
nod* inserareArbore(nod* radacina, rezervare r) {

	if (radacina) { // diferit de NULL

		if (r.id < radacina->info.id) {

			radacina->stanga = inserareArbore(radacina->stanga, r);
			return radacina;

		}
		else if (r.id > radacina->info.id) {

			radacina->dreapta = inserareArbore(radacina->dreapta, r);
			return radacina;

		}
		else {

			return radacina;

		}

	}

	return creareNod(r, NULL, NULL);

}

//Radacina Stanga Dreapta
void afisarePreordine(nod* radacina) {

	if (radacina) {

		printf("%u %s %s %.2f \n", radacina->info.id, radacina->info.hotel,
			radacina->info.client, radacina->info.suma);
		afisarePreordine(radacina->stanga);
		afisarePreordine(radacina->dreapta);

	}

}

//Stanga Radacina Dreapta
void afisareInordine(nod* radacina) {

	if (radacina) {

		afisareInordine(radacina->stanga);
		printf("%u %s %s %.2f \n", radacina->info.id, radacina->info.hotel,
			radacina->info.client, radacina->info.suma);
		afisareInordine(radacina->dreapta);

	}

}

//Stanga Dreapta Radacina
void afisarePostordine(nod* radacina) {

	if (radacina) {

		afisarePostordine(radacina->stanga);
		afisarePostordine(radacina->dreapta);
		printf("%u %s %s %.2f \n", radacina->info.id, radacina->info.hotel,
			radacina->info.client, radacina->info.suma);

	}

}

// Recursivitate
int maxim(int a, int b) {

	/*if (a > b) {

		return a;

	}
	else {

		return b;

	}*/

	return a > b ? a : b;
}

int nrNiveluri(nod* radacina) {

	if (radacina) {

		return 1 + maxim(nrNiveluri(radacina->stanga), nrNiveluri(radacina->dreapta));

	}
	else {

		return 0;

	}

}

void dezalocareArbore(nod* radacina) {

	if (radacina) {

		nod* tempStanga = radacina->stanga;
		nod* tempDreapta = radacina->dreapta;
		free(radacina->info.hotel);
		free(radacina->info.client);
		free(radacina);

		dezalocareArbore(tempStanga);
		dezalocareArbore(tempDreapta);

	}

}

void main() {

	//Structuri
	rezervare r;
	nod* radacina = NULL;


	//Auxiliare pentru citire
	char buffer[100];
	char separator[] = ","; // daca exista probleme in fisier .txt punem \n
	char* token;

	FILE* f = fopen("rezervari.txt", "r");
	while (fgets(buffer, 100, f)) {

		token = strtok(buffer, separator);
		r.id = atoi(token);

		token = strtok(NULL, separator);
		r.hotel = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(r.hotel, token);

		token = strtok(NULL, separator);
		r.client = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(r.client, token);

		token = strtok(NULL, separator);
		r.suma = atof(token);

		radacina = inserareArbore(radacina, r);

		free(r.hotel);
		free(r.client);

	}

	afisarePreordine(radacina);

	int nr = nrNiveluri(radacina);

	printf("\n%d\n", nr);

}