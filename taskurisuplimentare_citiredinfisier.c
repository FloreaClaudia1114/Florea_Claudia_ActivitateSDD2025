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

void afisareFarmacie(Farmacie farmacie) {
	printf("cod:%d\n", farmacie.cod);
	printf("denumire:%s\n", farmacie.denumire);
	printf("nrsucursale:%d\n", farmacie.nrsucursale);
	printf("denumire:%.2f\n", farmacie.pretmed);

}
void afisarevectorFarmacii(Farmacie* farmacii, int nrfarmacii) {
	for (int i = 0; i < nrfarmacii; i++) {
		afisareFarmacie(farmacii[i]);

	}

}

