#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM1 10000
#define TAM2 50000
#define TAM3 100000

void copiarVetor(int *origem, int *destino, int n) {
    for (int i = 0; i < n; i++)
        destino[i] = origem[i];
}

// ---------------------------
// Bubble Sort
// ---------------------------
long long bubbleSort(int *X, int n) {
    long long comparacoes = 0;
    int aux;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoes++;
            if (X[j] > X[j + 1]) {
                aux = X[j];
                X[j] = X[j + 1];
                X[j + 1] = aux;
            }
        }
    }
    return comparacoes;
}

// ---------------------------
// Insertion Sort
// ---------------------------
long long insertionSort(int *X, int n) {
    long long comparacoes = 0;
    int eleito, j;
    for (int i = 1; i < n; i++) {
        eleito = X[i];
        j = i - 1;
        while (j >= 0 && X[j] > eleito) {
            comparacoes++;
            X[j + 1] = X[j];
            j--;
        }
        if (j >= 0)
            comparacoes++; // última comparação que falha
        X[j + 1] = eleito;
    }
    return comparacoes;
}

// ---------------------------
// Selection Sort
// ---------------------------
long long selectionSort(int *X, int n) {
    long long comparacoes = 0;
    int menor, pos, aux;
    for (int i = 0; i < n - 1; i++) {
        menor = X[i];
        pos = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (X[j] < menor) {
                menor = X[j];
                pos = j;
            }
        }
        if (pos != i) {
            aux = X[i];
            X[i] = X[pos];
            X[pos] = aux;
        }
    }
    return comparacoes;
}

// ---------------------------
// Merge Sort
// ---------------------------
long long comparacoesMerge = 0;

void intercala(int X[], int inicio, int meio, int fim) {
    int *aux = (int *)malloc((fim - inicio + 1) * sizeof(int));
    int i = inicio, j = meio + 1, k = 0;
    while (i <= meio && j <= fim) {
        comparacoesMerge++;
        if (X[i] <= X[j]) aux[k++] = X[i++];
        else aux[k++] = X[j++];
    }
    while (i <= meio) aux[k++] = X[i++];
    while (j <= fim) aux[k++] = X[j++];
    for (i = inicio, k = 0; i <= fim; i++, k++) X[i] = aux[k];
    free(aux);
}

void mergeSortRec(int X[], int inicio, int fim) {
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        mergeSortRec(X, inicio, meio);
        mergeSortRec(X, meio + 1, fim);
        intercala(X, inicio, meio, fim);
    }
}

long long mergeSort(int *X, int n) {
    comparacoesMerge = 0;
    mergeSortRec(X, 0, n - 1);
    return comparacoesMerge;
}

int main() {
    srand(time(NULL));

    FILE *f = fopen("resultados.csv", "w");
    if (!f) {
        printf("Erro ao criar o arquivo CSV!\n");
        return 1;
    }

    fprintf(f, "Algoritmo,N,TempoSegundos,Comparacoes\n");

    int tamanhos[3] = {TAM1, TAM2, TAM3};

    for (int t = 0; t < 3; t++) {
        int n = tamanhos[t];
        int *original = malloc(n * sizeof(int));
        int *vetor = malloc(n * sizeof(int));

        for (int i = 0; i < n; i++)
            original[i] = rand() % 100000;

        // Testar cada algoritmo
        const char *algoritmos[] = {"Bubble Sort", "Insertion Sort", "Selection Sort", "Merge Sort"};
        for (int a = 0; a < 4; a++) {
            copiarVetor(original, vetor, n);
            clock_t inicio = clock();
            long long comparacoes = 0;

            if (a == 0) comparacoes = bubbleSort(vetor, n);
            else if (a == 1) comparacoes = insertionSort(vetor, n);
            else if (a == 2) comparacoes = selectionSort(vetor, n);
            else comparacoes = mergeSort(vetor, n);

            clock_t fim = clock();
            double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            fprintf(f, "%s,%d,%.6f,%lld\n", algoritmos[a], n, tempo, comparacoes);
            printf("%s N=%d concluído.\n", algoritmos[a], n);
        }

        free(original);
        free(vetor);
    }

    fclose(f);
    printf("\n Arquivo 'resultados.csv' gerado com sucesso!\n");
    return 0;
}
