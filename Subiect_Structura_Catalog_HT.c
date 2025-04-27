#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int idCatalog;
    char* denumireCurs;
    char* denumireFormatieStudiu;
    unsigned char numarStudenti;
    unsigned char* noteStudenti;
}Catalog;

typedef struct Nod {
    Catalog* info;
    struct Nod* next;
}Nod;

typedef struct HT {
    Nod** vector;
    int dimensiune;
} HT;

HT* initializareHT(int dimensiune) {
    HT* ht = (HT*)malloc(sizeof(HT));

    ht->dimensiune = dimensiune;
    ht->vector = (Nod**)malloc(ht->dimensiune * sizeof(Nod*));

    for (int i = 0; i < ht->dimensiune; i++) {
        ht->vector[i] = NULL;
    }

    return ht;
}
int hash(HT* ht, char* denumireCurs) {
    int suma = 0;
    for (int i = 0; i < strlen(denumireCurs); i++) {
        suma += denumireCurs[i];
    }
    return suma % ht->dimensiune;
}

Catalog* initializareCatalog(unsigned int idCatalog, char* denumireCurs, char* denumireFormatieStudiu, unsigned char numarStudenti, unsigned char* noteStudenti) {
    Catalog* catalog = (Catalog*)malloc(sizeof(Catalog));
    catalog->idCatalog = idCatalog;

    catalog->denumireCurs = (char*)malloc(strlen(denumireCurs) + 1);
    strcpy(catalog->denumireCurs, denumireCurs);

    catalog->denumireFormatieStudiu = (char*)malloc(strlen(denumireFormatieStudiu) + 1);
    strcpy(catalog->denumireFormatieStudiu, denumireFormatieStudiu);

    catalog->numarStudenti = numarStudenti;

    // Alocăm memorie pentru noteStudenti și le copiem
    catalog->noteStudenti = (unsigned char*)malloc(numarStudenti * sizeof(unsigned char));
    for (int i = 0; i < numarStudenti; i++) {
        catalog->noteStudenti[i] = noteStudenti[i];
    }

    return catalog;
}

Nod* initializareNod(Catalog* info) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));

    nod->info = info;
    nod->next = NULL;

    return nod;
}

HT* inserareInHT(HT* ht, Catalog* catalog) {
    if (ht->vector != NULL) {
        int pozitie = hash(ht, catalog->denumireCurs);
        Nod* nou = initializareNod(catalog);

        if (ht->vector[pozitie] == NULL) {
            ht->vector[pozitie] = nou;
        }
        else {
            Nod* aux = ht->vector[pozitie];
            while (aux->next != NULL) {
                aux = aux->next;
            }
            aux->next = nou;
        }
    }

    return ht;
}
void afisare(HT* ht) {
    if (ht != NULL) {
        for (int i = 0; i < ht->dimensiune; i++) {
            if (ht->vector[i] != NULL) {
                Nod* temp = ht->vector[i];
                while (temp != NULL) {
                    // Afișează informațiile principale
                    printf("Pozitie: %d \n", i);
                    printf("ID Catalog: %u, Denumire Curs: %s, Denumire Formatie Studiu: %s, Numar Studenti: %d, Note Studenti: ",
                        temp->info->idCatalog, temp->info->denumireCurs, temp->info->denumireFormatieStudiu, temp->info->numarStudenti);

                    // Afișează notele studenților
                    for (int j = 0; j < temp->info->numarStudenti; j++) {
                        printf("%hhu ", temp->info->noteStudenti[j]); // Afișează fiecare notă
                    }
                    printf("\n");


                    temp = temp->next;
                }
            }
        }
    }
}


void dezalocare(HT* ht) {
    if (ht->vector != NULL) {
        for (int i = 0; i < ht->dimensiune; i++) {
            Nod* temp = ht->vector[i];
            while (temp != NULL) {
                Nod* deSters = temp;
                temp = temp->next;
                free(deSters->info->denumireCurs);
                free(deSters->info->denumireFormatieStudiu);
                free(deSters->info->noteStudenti);
                free(deSters->info);
                free(deSters);
            }
        }
        free(ht->vector);
        free(ht);
    }
}
///functia care determina valoarea medie a notelor pt catalog specificat ca parametru prin idcatalog
///cautam in tabela ht dupa idcatalog, pt acel id calculam media notelor, absenti inseamna ca nu vom lua in considerare nota 0
float valoaremedienotecatalog(HT* ht, unsigned int idCatalog) {
    if (ht == NULL) {
        return 0.0f;
    }

    for (int i = 0; i < ht->dimensiune; i++) {
        Nod* temp = ht->vector[i];
        while (temp != NULL) {
            if (temp->info->idCatalog == idCatalog) {
                // Am găsit catalogul, calculez media
                unsigned int suma = 0;
                unsigned int numarNoteValide = 0;

                for (int j = 0; j < temp->info->numarStudenti; j++) {
                    if (temp->info->noteStudenti[j] > 0) {
                        suma += temp->info->noteStudenti[j];
                        numarNoteValide++;
                    }
                }

                if (numarNoteValide > 0) {
                    return (float)suma / numarNoteValide;
                }
                else {
                    return 0.0f;
                }
            }
            temp = temp->next;
        }
    }

    return 0.0f; // Dacă nu găsim catalogul
}

///functia ce determina nr de cataloage completate pt un curs a carui denumire este specificata ca parametru
unsigned int numarCataloageCompletate(HT* ht, char* denumireCurs) {
    if (ht == NULL || denumireCurs == NULL) {
        return 0;
    }

    unsigned int contor = 0;
    for (int i = 0; i < ht->dimensiune; i++) {
        Nod* temp = ht->vector[i];
        while (temp != NULL) {
            if (strcmp(temp->info->denumireCurs, denumireCurs) == 0) {
                contor++;
            }
            temp = temp->next;
        }
    }

    return contor;


}
///functia ce determina cea mai lunga lista de coliziuni/cel mai mare cluster de coliziuni 
///prima oara afisam lista de clustere

void afisarelista(Nod* lista) {
    while (lista != NULL) {
        printf("ID Catalog: %u, Denumire Curs: %s, Denumire Formatie Studiu: %s, Numar Studenti: %d, Note Studenti: ",
            lista->info->idCatalog, lista->info->denumireCurs, lista->info->denumireFormatieStudiu, lista->info->numarStudenti);

        for (int i = 0; i < lista->info->numarStudenti; i++) {
            printf("%hhu ", lista->info->noteStudenti[i]);
        }
        printf("\n");

        lista = lista->next;
    }
}

///functia ce determina cea mai lunga lista de coliziuni/cel mai mare cluster de coliziuni
Nod* clustermaxim(HT* ht) {
    Nod* maxim = NULL;
    int lungimemaximacluster = 0;
    for (int i = 0; i < ht->dimensiune; i++) {
        Nod* temp = ht->vector[i];
        int lungimecurenta = 0;
        while (temp != NULL) {
            lungimecurenta++;
            temp = temp->next;

        }
        if (lungimecurenta > lungimemaximacluster) {
            lungimemaximacluster = lungimecurenta;
            maxim = ht->vector[i];
        }

    }

    return maxim;
}

int main() {
    unsigned int idCatalog;
    char denumireCurs[50];
    char denumireFormatieStudiu[50];
    unsigned char numarStudenti;
    unsigned char* noteStudenti;

    HT* ht = initializareHT(20);
    FILE* f = fopen("cataloage.txt", "r");

    if (f) {  // Verifică dacă fișierul a fost deschis cu succes
        while (fscanf_s(f, "%u %49s %49s %hhu", &idCatalog, denumireCurs, 50, denumireFormatieStudiu, 50, &numarStudenti) == 4) {
            // Alocă memorie pentru noteStudenti, în funcție de numărul de studenți
            noteStudenti = (unsigned char*)malloc(numarStudenti * sizeof(unsigned char));

            // Citește notele pentru fiecare student
            for (int i = 0; i < numarStudenti; i++) {
                fscanf_s(f, "%hhu", &noteStudenti[i]);
            }

            // Creează un obiect Catalog folosind datele citite
            Catalog* catalog = initializareCatalog(idCatalog, denumireCurs, denumireFormatieStudiu, numarStudenti, noteStudenti);

            // Inserează catalogul în tabelul hash
            ht = inserareInHT(ht, catalog);

            // Eliberează memoria alocată pentru noteStudenti
            free(noteStudenti);
        }

        fclose(f);  // Închide fișierul după citire
    }
    else {
        printf("Eroare la deschiderea fisierului!\n");  // Mesaj de eroare dacă fișierul nu se poate deschide
    }

    // Afișează catalogul
    afisare(ht);

    printf("==============================================\n");


    ////functia valoaremedienotecatalog
    unsigned int idCautat = 1001;
    float medie = valoaremedienotecatalog(ht, idCautat);
    printf("Media notelor pentru catalogul cu ID %u este: %.2f\n", idCautat, medie);


    printf("\n==============================================\n");

    char numeCursCautat[50];
    printf("Introdu numele cursului pentru care vrei sa afli numarul de cataloage: ");
    scanf("%49s", numeCursCautat);

    unsigned int numar = numarCataloageCompletate(ht, numeCursCautat);
    printf("Numarul de cataloage completate pentru cursul %s este: %u\n", numeCursCautat, numar);

    printf("\n==============================================\n");

    /// afisam clustermaxim
    Nod* maxcluster = clustermaxim(ht);

    if (maxcluster == NULL) {
        printf("Nu exista niciun cluster!\n");
    }
    else {
        afisarelista(maxcluster);
    }



    // Dezalocă memoria utilizată
    dezalocare(ht);

    return 0;
}

