#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Farmacie {

	int cod;
	char* denumire;
	int nrsucursale;
	float pretmed;

}Farmacie;

struct Farmacie initializare(int cod, char* denumire, int nrsucursale, float pretmed) {
	struct Farmacie f;

	f.cod = cod;
	f.denumire = (char*)malloc(sizeof(char) + (strlen(denumire) + 1));
	strcpy_s(f.denumire, strlen(denumire) + 1, denumire);
	f.nrsucursale = nrsucursale;
	f.pretmed = pretmed;

	return f;
}

void afisare(struct Farmacie f) {
	if (f.denumire != NULL) {
		printf("%d. Farmacia %s are %2d sucursale si costa %.5f EURO\n",
			f.cod, f.denumire, f.nrsucursale, f.pretmed);
	}
	else {
		printf("%d. Farmacii sunt in total %2d sucursale si costa %.5f EURO\n",
			f.cod, f.nrsucursale, f.pretmed);
	}

}
///afisarea elementelor din vector apeland functia de afisare
void afisareVector(struct Farmacie* vector, int nrElemente) {
	for (int i = 0; i < nrElemente;i++) {
		afisare(vector[i]);

	}

}
///functie in care copiem intr-un vector nou pe care il von returna, primele n-elemente copiate

struct Farmacie* copiazaPrimeleNElemente(struct Farmacie* vector, int nrElemente, int nrElementeCopiate) {

	struct Farmacie* vectornou = NULL;
	vectornou = (struct Farmacie*)malloc(sizeof(struct Farmacie) * nrElementeCopiate);
	for (int i = 0;i < nrElementeCopiate; i++) {
		vectornou[i] = vector[i];
		vectornou[i].denumire = (char*)malloc(strlen(vector[i].denumire) + 1);

	}

	return vectornou;

}
///dezalocare
void dezalocare(struct Farmacie** vector,int* nrElemente ) {
	for (int i = 0; i < (*nrElemente); i++) {
		if ((*vector)[i].denumire != NULL){
			free((*vector)[i].denumire);
        }
		
    }
	free(*vector);
	*vector != NULL;

}
///functia ce copiaza medicamentele cele mai scumpe
void copiazaMedicamentelemaiScumpe(struct Farmacie* vector,char nrElemente,float pretminim, struct Farmacie** vectorNou, int* dimensiune  ) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pretmed >= pretminim) {
			(*dimensiune)++;

		}
		if ((*vectorNou != NULL)) {
			free(*vectorNou);
	    }
		*vectorNou = (struct Farmacie*)malloc(sizeof(struct Farmacie)*(*dimensiune));
		int k = 0;
		for (int i = 0; i < nrElemente; i++) {
			if (vector[i].pretmed >= pretminim) {
				(*vectorNou)[k] = vector[i];
				(*vectorNou)[k].denumire = (char*)malloc(strlen(vector[i].denumire) + 1);

		    }

		}

	}

}
int main() {
	struct Farmacie* vectorFarmacii = NULL;
	int nrFarmacii = 3;
	vectorFarmacii = (struct Farmacie*)malloc(sizeof(struct Farmacie) * nrFarmacii);
	vectorFarmacii[0] = initializare(1, "Catena", 20, 15.99);
	vectorFarmacii[1] = initializare(2, "Dona", 25, 29.99);
	vectorFarmacii[2] = initializare(3, "Iris", 66, 9.99);

	afisareVector(vectorFarmacii, nrFarmacii);
	struct Farmacie* primeleFarmacii = NULL;
	int nrPrimeleFarmacii = 2;
	primeleFarmacii = copiazaPrimeleNElemente(vectorFarmacii, nrFarmacii, nrPrimeleFarmacii);
	printf("\n\Primele telefoane:\n");
	afisareVector(primeleFarmacii, nrPrimeleFarmacii);
    dezalocare(&primeleFarmacii, &nrPrimeleFarmacii);

	return 0;


}


