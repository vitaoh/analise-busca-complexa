#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_10K 10000
#define N_50K 50000
#define N_100K 100000
#define NUM_TESTES 100

long long comparacoes;

typedef struct {
    int tamanho;
    double tempo_sequencial;
    double tempo_binaria;
    double comp_sequencial;
    double comp_binaria;
} Resultado;

// --- Algoritmos de busca ---
int buscaSequencial(int *vetor, int tamanho, int chave) {
    comparacoes = 0;
    for (int i = 0; i < tamanho; i++) {
        comparacoes++;
        if (vetor[i] == chave) {
            return i;
        }
    }
    return -1;
}

int buscaBinaria(int *vetor, int tamanho, int chave) {
    comparacoes = 0;
    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        comparacoes++; // Comparação do while
        int meio = inicio + (fim - inicio) / 2;

        comparacoes++; // Comparação com elemento do meio
        if (vetor[meio] == chave) {
            return meio;
        } else if (vetor[meio] < chave) {
            comparacoes++; // Segunda comparação
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}


// --- Funções auxiliares ---
int comparar(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void gerarVetor(int *vetor, int tamanho) {
    printf("  Gerando %d elementos aleatórios...\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand() % (tamanho * 2);
    }
}

void ordenarVetor(int *vetor, int tamanho) {
    printf("  Ordenando vetor...\n");
    qsort(vetor, tamanho, sizeof(int), comparar);
}

void criarChavesTeste(int *vetor, int tamanho, int *existentes, int *inexistentes) {
    printf("  Criando chaves de teste...\n");

    // 50 chaves existentes
    for (int i = 0; i < 50; i++) {
        int indice = rand() % tamanho;
        existentes[i] = vetor[indice];
    }

    // 50 chaves inexistentes
    for (int i = 0; i < 50; i++) {
        int chave;
        int existe;

        do {
            chave = rand() % (tamanho * 4) + tamanho * 2;
            existe = 0;
            for (int j = 0; j < tamanho; j++) {
                if (vetor[j] == chave) {
                    existe = 1;
                    break;
                }
            }
        } while (existe);

        inexistentes[i] = chave;
    }
}

// === FUNÇÃO PARA SALVAR RESULTADOS EM CSV ===
void salvarResultadoCSV(Resultado resultados[], int num_resultados) {
    FILE *arquivo = fopen("resultados.csv", "w");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar o arquivo CSV!\n");
        return;
    }

    // Cabeçalho do CSV
    fprintf(arquivo, "Tamanho,Tempo_Sequencial_ms,Tempo_Binaria_ms,Comp_Sequencial,Comp_Binaria,Speedup_Tempo,Reducao_Comparacoes\n");

    // Dados
    for (int i = 0; i < num_resultados; i++) {
        double speedup = resultados[i].tempo_sequencial / resultados[i].tempo_binaria;
        double reducao = resultados[i].comp_sequencial / resultados[i].comp_binaria;

        fprintf(arquivo, "%d,%.3f,%.3f,%.1f,%.1f,%.2f,%.2f\n",
                resultados[i].tamanho,
                resultados[i].tempo_sequencial,
                resultados[i].tempo_binaria,
                resultados[i].comp_sequencial,
                resultados[i].comp_binaria,
                speedup,
                reducao);
    }

    fclose(arquivo);
    printf("\n✅ Resultados salvos em 'resultados.csv'\n");
}

// === FUNÇÃO PRINCIPAL DE TESTE ===

Resultado rodarTestes(int tamanho) {
    printf("\n============================================\n");
    printf("TESTANDO COM N = %d\n", tamanho);
    printf("============================================\n");

    Resultado resultado = {0};
    resultado.tamanho = tamanho;

    // Aloca memória
    int *vetor = (int*)malloc(tamanho * sizeof(int));
    if (!vetor) {
        printf("Erro: Falha ao alocar memória!\n");
        return resultado;
    }

    int existentes[50], inexistentes[50];

    // Prepara dados
    gerarVetor(vetor, tamanho);
    criarChavesTeste(vetor, tamanho, existentes, inexistentes);

    // TESTE BUSCA SEQUENCIAL
    printf("\n[1/2] Executando Busca Sequencial...\n");
    clock_t inicio = clock();
    long long total_comp_seq = 0;

    for (int i = 0; i < 50; i++) {
        buscaSequencial(vetor, tamanho, existentes[i]);
        total_comp_seq += comparacoes;
    }
    for (int i = 0; i < 50; i++) {
        buscaSequencial(vetor, tamanho, inexistentes[i]);
        total_comp_seq += comparacoes;
    }

    clock_t fim = clock();
    resultado.tempo_sequencial = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
    resultado.comp_sequencial = (double)total_comp_seq / NUM_TESTES;

    // Prepara para busca binária
    ordenarVetor(vetor, tamanho);

    // TESTE BUSCA BINÁRIA
    printf("\n[2/2] Executando Busca Binária...\n");
    inicio = clock();
    long long total_comp_bin = 0;

    for (int i = 0; i < 50; i++) {
        buscaBinaria(vetor, tamanho, existentes[i]);
        total_comp_bin += comparacoes;
    }
    for (int i = 0; i < 50; i++) {
        buscaBinaria(vetor, tamanho, inexistentes[i]);
        total_comp_bin += comparacoes;
    }

    fim = clock();
    resultado.tempo_binaria = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
    resultado.comp_binaria = (double)total_comp_bin / NUM_TESTES;

    // RESULTADOS
    printf("\n--- RESULTADOS ---\n");
    printf("Busca Sequencial:\n");
    printf("  Tempo: %.3f ms\n", resultado.tempo_sequencial);
    printf("  Comparações médias: %.1f\n", resultado.comp_sequencial);

    printf("\nBusca Binária:\n");
    printf("  Tempo: %.3f ms\n", resultado.tempo_binaria);
    printf("  Comparações médias: %.1f\n", resultado.comp_binaria);

    printf("\nMelhoria da Busca Binária:\n");
    printf("  %.1fx mais rápida\n", resultado.tempo_sequencial / resultado.tempo_binaria);
    printf("  %.1fx menos comparações\n", resultado.comp_sequencial / resultado.comp_binaria);

    free(vetor);
    return resultado;
}

int main() {
    srand(time(NULL));

    Resultado resultados[3];
    resultados[0] = rodarTestes(N_10K);
    resultados[1] = rodarTestes(N_50K);
    resultados[2] = rodarTestes(N_100K);

    // Salva resultados no CSV
    salvarResultadoCSV(resultados, 3);

    printf("\n==============================================\n");
    printf("TESTES CONCLUÍDOS!\n");
    printf("\nPróximo passo:\n");
    printf("  Execute: python3 gerar_graficos.py\n");
    printf("==============================================\n");

    return 0;
}