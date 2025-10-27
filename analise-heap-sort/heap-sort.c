#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Variável global para contar comparações
long long int comparacoes = 0;

// Arrays para registrar resultados dos testes
#define NUM_TESTES 3
int tamanhos[NUM_TESTES] = {10000, 50000, 100000};
double tempos[NUM_TESTES];
long long comparacoes_arr[NUM_TESTES];

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void max_heapify(int arr[], int n, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    comparacoes++;
    if (esquerda < n && arr[esquerda] > arr[maior])
        maior = esquerda;
    comparacoes++;
    if (direita < n && arr[direita] > arr[maior])
        maior = direita;
    if (maior != i) {
        swap(&arr[i], &arr[maior]);
        max_heapify(arr, n, maior);
    }
}

void build_max_heap(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        max_heapify(arr, n, i);
    }
}

void heap_sort(int arr[], int n) {
    build_max_heap(arr, n);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        max_heapify(arr, i, 0);
    }
}

void gerar_dados(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }
}

void copiar_array(int origem[], int destino[], int n) {
    for (int i = 0; i < n; i++) {
        destino[i] = origem[i];
    }
}

int verificar_ordenacao(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void medir_desempenho(int n, int num_execucoes, int idx) {
    int *arr_original = (int *)malloc(n * sizeof(int));
    int *arr_copia = (int *)malloc(n * sizeof(int));
    if (arr_original == NULL || arr_copia == NULL) {
        printf("Erro ao alocar memória!\n");
        return;
    }
    double tempo_total = 0.0;
    long long int comparacoes_total = 0;

    for (int exec = 0; exec < num_execucoes; exec++) {
        gerar_dados(arr_original, n);
        copiar_array(arr_original, arr_copia, n);
        comparacoes = 0;
        clock_t inicio = clock();
        heap_sort(arr_copia, n);
        clock_t fim = clock();
        double tempo_exec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        tempo_total += tempo_exec;
        comparacoes_total += comparacoes;
        if (!verificar_ordenacao(arr_copia, n)) {
            printf("ERRO: Array não foi ordenado corretamente na execução %d!\n", exec + 1);
        }
    }
    tempos[idx] = tempo_total / num_execucoes;
    comparacoes_arr[idx] = comparacoes_total / num_execucoes;

    free(arr_original);
    free(arr_copia);
}

void gerar_csv() {
    FILE *f = fopen("resultados-heap-sort.csv", "w");
    if (f == NULL) {
        printf("Erro ao criar arquivo CSV\n");
        return;
    }
    fprintf(f, "Algoritmo,N,Tempo_Medio_segundos,Numero_Medio_Comparacoes,Complexidade_Teorica,Razao_Pratico_Teorico\n");
    for (int i = 0; i < NUM_TESTES; i++) {
        double comp_teorico = tamanhos[i]*log2(tamanhos[i]);
        double razao = comparacoes_arr[i]/comp_teorico;
        fprintf(f, "Heap Sort,%d,%.6f,%lld,%.0f,%.2f\n",
                tamanhos[i], tempos[i], comparacoes_arr[i], comp_teorico, razao);
    }
    fclose(f);
    printf("Arquivo CSV gerado: resultados-heap-sort.csv\n");
}

int main() {
    srand(time(NULL));
    int num_execucoes = 10;
    for (int i = 0; i < NUM_TESTES; i++) {
        medir_desempenho(tamanhos[i], num_execucoes, i);
    }
    gerar_csv();
    return 0;
}
