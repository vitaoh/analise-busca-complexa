#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 11  // 10 vértices + 1 (índice 0 não usado)

typedef struct vertice {
    int num;
    struct vertice *prox;
} VERTICE;

typedef struct listaadj {
    VERTICE *listav;
} LISTAADJ;

typedef struct queue {
    int numv;
    struct queue *prox;
} QUEUE;

LISTAADJ Adj[MAX];
int marcado[MAX], dist[MAX], pai[MAX];
int tam = 10;  // Grafo fixo com 10 vértices

// Mapeamento Nome ↔ ID
char *nomes[MAX] = {
    "",
    "Alice",
    "Bruno",
    "Carla",
    "David",
    "Elias",
    "Flavia",
    "Gabriel",
    "Helena",
    "Igor",
    "Julia"
};

// Funções da Fila (BFS usa fila, não pilha)
void enqueue(QUEUE **inicio, QUEUE **fim, int n) {
    QUEUE *novo = (QUEUE *)malloc(sizeof(QUEUE));
    novo->numv = n;
    novo->prox = NULL;
    if (*fim == NULL) {
        *inicio = novo;
        *fim = novo;
    } else {
        (*fim)->prox = novo;
        *fim = novo;
    }
}

int dequeue(QUEUE **inicio, QUEUE **fim) {
    if (*inicio == NULL) return -1;
    QUEUE *aux = *inicio;
    int v = aux->numv;
    *inicio = aux->prox;
    if (*inicio == NULL) *fim = NULL;
    free(aux);
    return v;
}

// Converter nome para ID
int nomeParaID(char *nome) {
    for (int i = 1; i <= tam; i++) {
        if (strcmp(nomes[i], nome) == 0) return i;
    }
    return -1;
}

// Construir grafo fixo conforme o PDF
void construirGrafoFixo() {
    // Inicializar listas
    for (int i = 1; i <= tam; i++) Adj[i].listav = NULL;
    
    // Conexões conforme especificado no PDF
    // Alice (1) -> Bruno(2), Carla(3)
    VERTICE *novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 2; novo->prox = Adj[1].listav; Adj[1].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 3; novo->prox = Adj[1].listav; Adj[1].listav = novo;
    
    // Bruno (2) -> Alice(1), David(4), Elias(5)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 1; novo->prox = Adj[2].listav; Adj[2].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 4; novo->prox = Adj[2].listav; Adj[2].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 5; novo->prox = Adj[2].listav; Adj[2].listav = novo;
    
    // Carla (3) -> Alice(1), Elias(5), Flávia(6)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 1; novo->prox = Adj[3].listav; Adj[3].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 5; novo->prox = Adj[3].listav; Adj[3].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 6; novo->prox = Adj[3].listav; Adj[3].listav = novo;
    
    // David (4) -> Bruno(2), Gabriel(7)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 2; novo->prox = Adj[4].listav; Adj[4].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 7; novo->prox = Adj[4].listav; Adj[4].listav = novo;
    
    // Elias (5) -> Bruno(2), Carla(3), Flávia(6)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 2; novo->prox = Adj[5].listav; Adj[5].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 3; novo->prox = Adj[5].listav; Adj[5].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 6; novo->prox = Adj[5].listav; Adj[5].listav = novo;
    
    // Flávia (6) -> Carla(3), Elias(5), Helena(8)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 3; novo->prox = Adj[6].listav; Adj[6].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 5; novo->prox = Adj[6].listav; Adj[6].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 8; novo->prox = Adj[6].listav; Adj[6].listav = novo;
    
    // Gabriel (7) -> David(4), Igor(9)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 4; novo->prox = Adj[7].listav; Adj[7].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 9; novo->prox = Adj[7].listav; Adj[7].listav = novo;
    
    // Helena (8) -> Flávia(6), Júlia(10)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 6; novo->prox = Adj[8].listav; Adj[8].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 10; novo->prox = Adj[8].listav; Adj[8].listav = novo;
    
    // Igor (9) -> Gabriel(7), Júlia(10)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 7; novo->prox = Adj[9].listav; Adj[9].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 10; novo->prox = Adj[9].listav; Adj[9].listav = novo;
    
    // Júlia (10) -> Helena(8), Igor(9)
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 8; novo->prox = Adj[10].listav; Adj[10].listav = novo;
    novo = (VERTICE*)malloc(sizeof(VERTICE)); novo->num = 9; novo->prox = Adj[10].listav; Adj[10].listav = novo;
}

// Algoritmo BFS principal
int bfs(int origem, int destino) {
    QUEUE *inicio = NULL, *fim = NULL;
    
    // Inicializar arrays
    for (int i = 1; i <= tam; i++) {
        marcado[i] = 0;
        dist[i] = -1;
        pai[i] = -1;
    }
    
    // Começar pela origem
    marcado[origem] = 1;
    dist[origem] = 0;
    enqueue(&inicio, &fim, origem);
    
    while (inicio != NULL) {
        int v = dequeue(&inicio, &fim);
        VERTICE *vert = Adj[v].listav;
        
        while (vert != NULL) {
            int w = vert->num;
            if (!marcado[w]) {
                marcado[w] = 1;
                dist[w] = dist[v] + 1;
                pai[w] = v;
                enqueue(&inicio, &fim, w);
                
                if (w == destino) {
                    // Limpar fila
                    while (inicio != NULL) dequeue(&inicio, &fim);
                    return dist[w];
                }
            }
            vert = vert->prox;
        }
    }
    return -1;  // Sem caminho
}

// Reconstruir e imprimir caminho
void imprimirCaminho(int origem, int destino) {
    if (destino == origem) {
        printf("%s", nomes[origem]);
        return;
    }
    imprimirCaminho(origem, pai[destino]);
    printf(" -> %s", nomes[destino]);
}

int main() {
    construirGrafoFixo();
    char nomeOrigem[50], nomeDestino[50];
    int op;
    
    printf("=== BUSCA EM LARGURA (BFS) EM REDE SOCIAL ===\n");
    
    do {
        printf("\n--- MENU ---\n");
        printf("1. Buscar conexão entre duas pessoas\n");
        printf("2. Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);
        
        if (op == 1) {
            printf("\nDigite o nome da pessoa de origem: ");
            scanf("%s", nomeOrigem);
            printf("Digite o nome da pessoa de destino: ");
            scanf("%s", nomeDestino);
            
            int idOrigem = nomeParaID(nomeOrigem);
            int idDestino = nomeParaID(nomeDestino);
            
            if (idOrigem == -1 || idDestino == -1) {
                printf("Nome(s) inválido(s)!\n");
                continue;
            }
            
            int distancia = bfs(idOrigem, idDestino);
            
            if (distancia == -1) {
                printf("Não há conexão entre %s e %s.\n", nomeOrigem, nomeDestino);
            } else {
                printf("\n✓ Conexão encontrada!\n");
                printf("• Grau de separação: %d\n", distancia);
                printf("• Caminho mais curto: ");
                imprimirCaminho(idOrigem, idDestino);
                printf("\n");
            }
        }
    } while (op != 2);
    
    // Liberar memória
    for (int i = 1; i <= tam; i++) {
        VERTICE *v = Adj[i].listav;
        while (v != NULL) {
            VERTICE *temp = v;
            v = v->prox;
            free(temp);
        }
    }
    
    printf("\nPrograma encerrado.\n");
    return 0;
}