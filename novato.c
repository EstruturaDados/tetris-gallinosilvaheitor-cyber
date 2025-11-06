#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definicao da capacidade maxima da fila de pecas futuras
#define CAPACIDADE_MAXIMA 5

// Variavel global para o ID unico das pecas
int proximo_id = 0;

// --- Estruturas de Dados ---

// Atributos das pecas: nome (tipo) e id
typedef struct {
    char nome; // Tipo da peca: 'I', 'O', 'T', 'L', 'J', 'S', 'Z'
    int id;    // Identificador unico
} Peca;

// Estrutura para a Fila Estatica (Circular/Linear Simplificada)
typedef struct {
    Peca elementos[CAPACIDADE_MAXIMA]; // Array para armazenar as pecas
    int inicio;                        // Indice da primeira peca (saida/dequeue)
    int fim;                           // Indice da ultima peca (entrada/enqueue)
    int tamanho_atual;                 // Numero atual de elementos na fila
} Fila;

// --- Prototipos de Funcoes ---
void inicializarFila(Fila *f);
Peca gerarPeca();
void exibirFila(Fila *f);
int estaVazia(Fila *f);
int estaCheia(Fila *f);
int enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
void menuAcoes(Fila *fila_pecas);

// --- Implementacao das Funcoes ---

/**
 * @brief Inicializa a fila definindo inicio, fim e tamanho_atual para zero.
 * @param f Ponteiro para a estrutura Fila.
 */
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1; // -1 indica que a fila esta vazia e o proximo item entra no indice 0
    f->tamanho_atual = 0;
}

/**
 * @brief Gera uma nova peca com um tipo aleatorio e um ID unico.
 * @return Retorna a estrutura Peca gerada.
 */
Peca gerarPeca() {
    Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Escolhe um tipo aleatorio
    nova_peca.nome = tipos[rand() % num_tipos];
    
    // Atribui o ID unico e incrementa o contador global
    nova_peca.id = proximo_id++;
    
    return nova_peca;
}

/**
 * @brief Verifica se a fila esta vazia.
 * @param f Ponteiro para a estrutura Fila.
 * @return 1 se vazia, 0 caso contrario.
 */
int estaVazia(Fila *f) {
    return (f->tamanho_atual == 0);
}

/**
 * @brief Verifica se a fila esta cheia.
 * @param f Ponteiro para a estrutura Fila.
 * @return 1 se cheia, 0 caso contrario.
 */
int estaCheia(Fila *f) {
    return (f->tamanho_atual == CAPACIDADE_MAXIMA);
}

/**
 * @brief Insere uma nova peca no final da fila (enqueue).
 * A logica de fila linear (nao-circular) e simplificada para fins didaticos.
 * @param f Ponteiro para a estrutura Fila.
 * @param p A peca a ser inserida.
 * @return 1 se a insercao for bem-sucedida, 0 caso a fila esteja cheia.
 */
int enqueue(Fila *f, Peca p) {
    if (estaCheia(f)) {
        printf("\n❌ ERRO: A fila de pecas futuras esta cheia (%d/%d). Nao e possivel inserir.\n", f->tamanho_atual, CAPACIDADE_MAXIMA);
        return 0;
    }
    
    // Como a fila e linear, o novo elemento sempre entra na proxima posicao apos o ultimo elemento (fim)
    f->fim++;
    f->elementos[f->fim] = p;
    f->tamanho_atual++;
    
    printf("\n✅ Inserido (ENQUEUE): Peca [%c %d] adicionada ao FINAL da fila.\n", p.nome, p.id);
    return 1;
}

/**
 * @brief Remove a peca da frente da fila (dequeue).
 * Implementa um shift simples de todos os elementos apos a remocao.
 * @param f Ponteiro para a estrutura Fila.
 * @return A peca removida. Retorna uma peca vazia se a fila estiver vazia.
 */
Peca dequeue(Fila *f) {
    Peca peca_removida = {' ', -1}; // Inicializa peca vazia para retorno de erro
    
    if (estaVazia(f)) {
        printf("\n❌ ERRO: A fila de pecas futuras esta vazia. Nao ha pecas para 'jogar'.\n");
        return peca_removida;
    }
    
    // 1. Pega a peca do inicio (indice 'inicio')
    peca_removida = f->elementos[f->inicio];
    
    // 2. Desloca todos os elementos uma posicao para a frente (simulando a remocao do inicio)
    for (int i = f->inicio; i < f->fim; i++) {
        f->elementos[i] = f->elementos[i + 1];
    }
    
    // 3. Ajusta os ponteiros e o tamanho
    f->fim--; // O ultimo elemento valido agora e a posicao anterior
    f->tamanho_atual--;
    
    printf("\n✅ Removido (DEQUEUE): Peca [%c %d] foi 'jogada' (removida do INICIO).\n", peca_removida.nome, peca_removida.id);
    return peca_removida;
}

/**
 * @brief Exibe o estado atual da fila de pecas.
 * @param f Ponteiro para a estrutura Fila.
 */
void exibirFila(Fila *f) {
    printf("\n--- 🧩 FILA DE PECAS FUTURAS (Capacidade: %d) 🧩 ---\n", CAPACIDADE_MAXIMA);
    
    if (estaVazia(f)) {
        printf(">>> FILA VAZIA <<<\n");
        printf("Tamanho atual: 0\n");
        return;
    }

    // Exibe o formato 'Tetris Stack'
    printf("Fila de Pecas: ");
    for (int i = f->inicio; i <= f->fim; i++) {
        // [T 0]
        printf("[%c %d]", f->elementos[i].nome, f->elementos[i].id);
        if (i < f->fim) {
            printf(" -> ");
        }
    }
    printf("\n");
    
    // Exibe a informacao de estado da fila
    printf("Tamanho atual: %d\n", f->tamanho_atual);
    printf("Proxima a sair (INICIO): [%c %d]\n", f->elementos[f->inicio].nome, f->elementos[f->inicio].id);
    printf("Ultima a entrar (FIM): [%c %d]\n", f->elementos[f->fim].nome, f->elementos[f->fim].id);
    printf("------------------------------------------------------\n");
}


// --- Funcao Principal e Menu ---

/**
 * @brief Gerencia o menu de acoes e o fluxo do programa.
 * @param fila_pecas Ponteiro para a fila principal.
 */
void menuAcoes(Fila *fila_pecas) {
    int escolha;
    
    do {
        // Exibe o estado atual da fila
        exibirFila(fila_pecas);

        printf("\n================ OPCOES DE ACAO (FIFO) ================\n");
        printf("Codigo | Acao\n");
        printf("-------------------------------------------------------\n");
        printf("  1    | Jogar peca (DEQUEUE - Remove do INICIO)\n");
        printf("  2    | Inserir nova peca (ENQUEUE - Adiciona ao FINAL)\n");
        printf("  0    | Sair\n");
        printf("=======================================================\n");
        printf("Escolha o Codigo da Acao: ");
        
        if (scanf("%d", &escolha) != 1) {
            printf("\n[ERRO] Entrada invalida. Por favor, digite um numero.\n");
            // Limpa o buffer de entrada
            while (getchar() != '\n'); 
            continue;
        }

        Peca peca_removida;
        Peca nova_peca;

        switch (escolha) {
            case 1:
                // DEQUEUE: Jogar a peça
                peca_removida = dequeue(fila_pecas);
                break;
                
            case 2:
                // ENQUEUE: Inserir nova peça
                nova_peca = gerarPeca();
                enqueue(fila_pecas, nova_peca);
                break;
                
            case 0:
                printf("\n👋 Jogo Tetris Stack Encerrado. Ate a proxima rodada!\n");
                break;
                
            default:
                printf("\n[ALERTA] Opcao invalida. Tente novamente.\n");
                break;
        }
        
    } while (escolha != 0);
}

/**
 * @brief Funcao principal do programa.
 */
int main() {
    // Inicializa o gerador de numeros aleatorios para as pecas
    srand(time(NULL));

    // Inicializa a fila de pecas
    Fila fila_pecas;
    inicializarFila(&fila_pecas);

    // 1. Inicializar a fila de pecas com um numero fixo de elementos (Ex: 5)
    // Preenche a fila inicial
    printf("Iniciando Tetris Stack: Preenchendo Fila Inicial (Capacidade %d)\n", CAPACIDADE_MAXIMA);
    while (!estaCheia(&fila_pecas)) {
        Peca p = gerarPeca();
        // Apenas insere sem exibir a mensagem de sucesso no inicio
        fputs("", stdout); // Truque para evitar aviso de retorno nao-usado de enqueue
        enqueue(&fila_pecas, p);
    }
    printf("\nFila inicial preenchida. Prepare-se para jogar!\n");
    
    // Inicia o menu de acoes
    menuAcoes(&fila_pecas);

    return 0;
}