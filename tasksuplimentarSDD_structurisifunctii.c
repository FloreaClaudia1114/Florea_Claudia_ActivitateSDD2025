
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
void modificaPret(struct Farmacie* f, float noulpretmed) {
	if (noulpretmed > 0) {
		f->pretmed = noulpretmed;

	}

}
void dezalocare(struct Farmacie* f) {
	if (f->denumire = NULL) {
		free(f->denumire);
		f->denumire = NULL;
	}
	else {

	}

}
float sumaPreturi(struct Farmacie farmacii[], int n) {
	float suma = 0;
	for (int i = 0; i < n; i++) {
		suma += farmacii[i].pretmed;
	}
	return suma;
}
int main() {
	struct Farmacie f;
	f = initializare(1, "Catena", 30, 27.5);
	afisare(f);
	modificaPret(&f, 12);

	afisare(f);
	dezalocare(&f);
	afisare(f);
	struct Farmacie farmacii[] = {
		{1, "Catena", 20, 15.99},
		{2, "Sensiblu", 30, 12.50},
		{3, "Dona", 25, 18.75},
		{4, "HelpNet", 10, 9.99}
	};
	int n = sizeof(farmacii) / sizeof(farmacii[0]);

	printf("Suma preturilor: %.2f\n", sumaPreturi(farmacii, n));

	return 0;


}


