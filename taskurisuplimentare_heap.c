#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACIENTI 100

typedef struct Pacient {
    char nume[100];
    int varsta;
    int grad_urgenta;
} Pacient;


typedef struct {
    Pacient* heap[MAX_PACIENTI];
    int dim;
} MaxHeap;


Pacient* creaza_pacient(const char* nume, int varsta, int grad_urgenta) {
    Pacient* pacient = (Pacient*)malloc(sizeof(Pacient));
    strncpy(pacient->nume, nume, 100);
    pacient->varsta = varsta;
    pacient->grad_urgenta = grad_urgenta;
    return pacient;
}


void schimba_pacienti(Pacient** p1, Pacient** p2) {
    Pacient* temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}


void heapify(MaxHeap* heap, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;


    if (left < heap->dim && heap->heap[left]->grad_urgenta > heap->heap[largest]->grad_urgenta) {
        largest = left;
    }


    if (right < heap->dim && heap->heap[right]->grad_urgenta > heap->heap[largest]->grad_urgenta) {
        largest = right;
    }


    if (largest != idx) {
        schimba_pacienti(&heap->heap[idx], &heap->heap[largest]);
        heapify(heap, largest);
    }
}

// Functia de inserare a unui pacient in Max-Heap
void inserare_pacient(MaxHeap* heap, Pacient* pacient) {
    if (heap->dim >= MAX_PACIENTI) {
        printf("Heap-ul este plin!\n");
        return;
    }


    heap->heap[heap->dim] = pacient;
    int idx = heap->dim;
    heap->dim++;


    while (idx != 0 && heap->heap[(idx - 1) / 2]->grad_urgenta < heap->heap[idx]->grad_urgenta) {
        schimba_pacienti(&heap->heap[idx], &heap->heap[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

// Functia pentru extragerea pacientului cu cea mai mare urgenta
Pacient* extrage_max(MaxHeap* heap) {
    if (heap->dim <= 0) {
        printf("Heap-ul este gol!\n");
        return NULL;
    }


    Pacient* max_pacient = heap->heap[0];


    heap->heap[0] = heap->heap[heap->dim - 1];
    heap->dim--;
    heapify(heap, 0);

    return max_pacient;
}

// Functia de afisare a pacientilor
void afiseaza_pacienti(MaxHeap* heap) {
    printf("\nPacienti in ordinea urgentei:\n");
    for (int i = 0; i < heap->dim; i++) {
        printf("Nume: %s, Varsta: %d, Grad urgenta: %d\n", heap->heap[i]->nume, heap->heap[i]->varsta, heap->heap[i]->grad_urgenta);
    }
}

// Functia principala
int main() {
    MaxHeap heap;
    heap.dim = 0;


    inserare_pacient(&heap, creaza_pacient("Ion Popescu", 45, 5));
    inserare_pacient(&heap, creaza_pacient("Maria Ionescu", 30, 3));
    inserare_pacient(&heap, creaza_pacient("George Vasile", 60, 8));
    inserare_pacient(&heap, creaza_pacient("Ana Dragomir", 25, 1));
    inserare_pacient(&heap, creaza_pacient("Radu Gheorghe", 40, 6));


    afiseaza_pacienti(&heap);


    printf("\nExtragem pacienti in ordinea urgentei:\n");
    Pacient* pacient = NULL;
    while ((pacient = extrage_max(&heap)) != NULL) {
        printf("Preluat: %s, Grad urgenta: %d\n", pacient->nume, pacient->grad_urgenta);
        free(pacient);
    }

    return 0;
}
