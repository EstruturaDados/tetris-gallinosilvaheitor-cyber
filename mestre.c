#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definições de Capacidade
#define CAP_FILA 5   // Capacidade fixa da fila circular
#define CAP_PILHA 3  // Capacidade maxima da pilha de reserva

// Variavel global para o ID unico das pecas
int proximo_id = 0;

// --- Estruturas de Dados ---

// Atributos das pecas: nome (tipo) e id
typedef struct {
    char nome; // Tipo da peca: 'I', 'O', 'T', 'L', 'J', 'S', 'Z'
    int id;    // Identificador unico
} Peca;

// Estrutura para a Fila Circular
typedef struct {
    Peca elementos[CAP_FILA];
    int inicio; 
    int fim;    
    int tamanho_atual;
} Fila;

// Estrutura para a Pilha Estatica (Peças Reservadas)
typedef struct {
    Peca elementos[CAP_PILHA];
    int topo; // Indice do ultimo elemento (topo da pilha)
} Pilha;

// --- Prototipos de Funcoes de Utilitario ---
Peca gerarPeca();
void exibirEstado(Fila *f, Pilha *p);

// --- Prototipos de Funcoes da Fila ---
void inicializarFila(Fila *f);
int fila_estaVazia(Fila *f);
int fila_estaCheia(Fila *f);
int enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);

// --- Prototipos de Funcoes da Pilha ---
void inicializarPilha(Pilha *p);
int pilha_estaVazia(Pilha *p);
int pilha_estaCheia(Pilha *p);
int push(Pilha *p, Peca peca);
Peca pop(Pilha *p);

// --- Prototipos de Funcoes de Acao Estrategica ---
void jogarPecaAcao(Fila *f);
void reservarPecaAcao(Fila *f, Pilha *p);
void usarPecaReservadaAcao(Pilha *p, Fila *f);
void trocarPecaUnicaAcao(Fila *f, Pilha *p);
void trocarPecasMultiplaAcao(Fila *f, Pilha *p);
void menuPrincipal(Fila *f, Pilha *p);


// =========================================================================
//                       IMPLEMENTACAO DAS FUNCOES
// =========================================================================

// --- 1. Funcoes de Utilitario ---

/**
 * @brief Gera uma nova peca com um tipo aleatorio e um ID unico.
 * @return Retorna a estrutura Peca gerada.
 */
Peca gerarPeca() {
    Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Inicializa o gerador de numeros aleatorios (garantindo que seja chamado uma vez)
    static int rand_inicializado = 0;
    if (!rand_inicializado) {
        srand(time(NULL));
        rand_inicializado = 1;
    }

    // Escolhe um tipo aleatorio
    nova_peca.nome = tipos[rand() % num_tipos];
    
    // Atribui o ID unico e incrementa o contador global
    nova_peca.id = proximo_id++;
    
    return nova_peca;
}

/**
 * @brief Exibe o estado atual da fila e da pilha.
 * @param f Ponteiro para a Fila.
 * @param p Ponteiro para a Pilha.
 */
void exibirEstado(Fila *f, Pilha *p) {
    // --- Visualizacao da Fila ---
    printf("\n--- 🧩 ESTADO ATUAL DAS ESTRUTURAS 🧩 ---\n");
    printf("Fila de Pecas (Frente -> Tras) [%d/%d]: ", f->tamanho_atual, CAP_FILA);
    if (fila_estaVazia(f)) {
        printf("[VAZIA]");
    } else {
        for (int i = 0; i < f->tamanho_atual; i++) {
            // Calcula o indice na fila circular
            int idx = (f->inicio + i) % CAP_FILA;
            printf("[%c %d]", f->elementos[idx].nome, f->elementos[idx].id);
            if (i < f->tamanho_atual - 1) printf(" -> ");
        }
    }

    // --- Visualizacao da Pilha ---
    printf("\nPilha de Reserva (Topo -> Base) [%d/%d]: ", p->topo + 1, CAP_PILHA);
    if (pilha_estaVazia(p)) {
        printf("[VAZIA]");
    } else {
        // Itera do topo (maior indice) ate a base (indice 0)
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d]", p->elementos[i].nome, p->elementos[i].id);
            if (i > 0) printf(" -> ");
        }
    }
    printf("\n--------------------------------------------\n");
}


// --- 2. Implementacao da Fila Circular ---

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1; // Com a fila circular, -1 indica que nao ha elementos
    f->tamanho_atual = 0;
}

int fila_estaVazia(Fila *f) {
    return (f->tamanho_atual == 0);
}

int fila_estaCheia(Fila *f) {
    return (f->tamanho_atual == CAP_FILA);
}

/**
 * @brief Insere uma nova peca no final da fila (enqueue).
 */
int enqueue(Fila *f, Peca p) {
    if (fila_estaCheia(f)) {
        return 0; // Falha: Fila Cheia
    }
    
    f->fim = (f->fim + 1) % CAP_FILA;
    f->elementos[f->fim] = p;
    f->tamanho_atual++;
    return 1; // Sucesso
}

/**
 * @brief Remove a peca da frente da fila (dequeue).
 */
Peca dequeue(Fila *f) {
    Peca peca_removida = {' ', -1}; // Peca de erro
    
    if (fila_estaVazia(f)) {
        return peca_removida; // Falha: Fila Vazia
    }
    
    peca_removida = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % CAP_FILA;
    f->tamanho_atual--;

    // Auto-reabastecimento: tenta inserir uma nova peça
    if (f->tamanho_atual < CAP_FILA) {
        Peca nova = gerarPeca();
        if (enqueue(f, nova)) {
             printf("[REABASTECIMENTO] Nova peca [%c %d] adicionada ao final da fila.\n", nova.nome, nova.id);
        }
    }
    return peca_removida;
}

// --- 3. Implementacao da Pilha Estatica ---

void inicializarPilha(Pilha *p) {
    p->topo = -1; // -1 indica que a pilha esta vazia
}

int pilha_estaVazia(Pilha *p) {
    return (p->topo == -1);
}

int pilha_estaCheia(Pilha *p) {
    return (p->topo == CAP_PILHA - 1);
}

/**
 * @brief Insere uma peca no topo da pilha (push).
 */
int push(Pilha *p, Peca peca) {
    if (pilha_estaCheia(p)) {
        return 0; // Falha: Pilha Cheia
    }
    
    p->topo++;
    p->elementos[p->topo] = peca;
    return 1; // Sucesso
}

/**
 * @brief Remove uma peca do topo da pilha (pop).
 */
Peca pop(Pilha *p) {
    Peca peca_removida = {' ', -1}; // Peca de erro
    
    if (pilha_estaVazia(p)) {
        return peca_removida; // Falha: Pilha Vazia
    }
    
    peca_removida = p->elementos[p->topo];
    p->topo--;
    return peca_removida;
}


// --- 4. Funcoes de Acao Estrategica (Logica do Jogo) ---

/**
 * @brief Joga a peca da frente da fila (dequeue).
 */
void jogarPecaAcao(Fila *f) {
    Peca p = dequeue(f);
    if (p.id != -1) {
        printf("\n✅ ACAO: Peca [%c %d] foi JOGADA (removida da frente da fila).\n", p.nome, p.id);
    } else {
        printf("\n❌ ERRO: Nao e possivel jogar. Fila de pecas futuras esta vazia.\n");
    }
}

/**
 * @brief Move a peca da frente da fila para o topo da pilha (Reservar).
 */
void reservarPecaAcao(Fila *f, Pilha *p) {
    if (pilha_estaCheia(p)) {
        printf("\n❌ ERRO: A Pilha de Reserva esta CHEIA (%d/%d). Nao e possivel reservar.\n", CAP_PILHA, CAP_PILHA);
        return;
    }
    if (fila_estaVazia(f)) {
        printf("\n❌ ERRO: A Fila de pecas esta VAZIA. Nao ha o que reservar.\n");
        return;
    }
    
    // 1. Remove da frente da fila
    Peca peca_fila = dequeue(f);
    
    // 2. Coloca no topo da pilha
    if (push(p, peca_fila)) {
        printf("\n✅ ACAO: Peca [%c %d] foi RESERVADA (movida da Fila para a Pilha).\n", peca_fila.nome, peca_fila.id);
    } 
    // OBS: O dequeue ja tenta reabastecer a fila automaticamente.
}

/**
 * @brief Remove a peca do topo da pilha (Usar Pecas Reservadas).
 */
void usarPecaReservadaAcao(Pilha *p, Fila *f) {
    Peca peca_pilha = pop(p);
    
    if (peca_pilha.id != -1) {
        printf("\n✅ ACAO: Peca [%c %d] foi USADA (removida do topo da Pilha).\n", peca_pilha.nome, peca_pilha.id);
    } else {
        printf("\n❌ ERRO: Nao e possivel usar. Pilha de reserva esta vazia.\n");
    }
}

/**
 * @brief Substitui a peca da frente da fila com o topo da pilha.
 * O elemento removido da fila vai para o topo da pilha.
 */
void trocarPecaUnicaAcao(Fila *f, Pilha *p) {
    if (pilha_estaVazia(p)) {
        printf("\n❌ ERRO: Pilha de reserva vazia. Nao ha o que trocar.\n");
        return;
    }
    if (fila_estaVazia(f)) {
         printf("\n❌ ERRO: Fila de pecas vazia. Nao ha o que trocar.\n");
        return;
    }
    
    // 1. Pega a peca da frente da fila, mas NAO a remove (peek simplificado)
    Peca peca_fila_frente = f->elementos[f->inicio];
    
    // 2. Remove a peca do topo da pilha (pop)
    Peca peca_pilha_topo = pop(p);
    
    // 3. Coloca a peca do topo da pilha na frente da fila
    f->elementos[f->inicio] = peca_pilha_topo;
    
    // 4. Coloca a peca que estava na frente da fila no topo da pilha (push)
    if (push(p, peca_fila_frente)) {
        printf("\n✅ ACAO: TROCA UNICA realizada.\n");
        printf("  - Fila (frente): [%c %d] -> [%c %d]\n", peca_fila_frente.nome, peca_fila_frente.id, peca_pilha_topo.nome, peca_pilha_topo.id);
        printf("  - Pilha (topo): [%c %d] -> [%c %d]\n", peca_pilha_topo.nome, peca_pilha_topo.id, peca_fila_frente.nome, peca_fila_frente.id);
    } else {
        // Isso nao deveria acontecer se a pilha nao estava vazia, mas e um bom guardrail
        printf("\n❌ ERRO: Falha no re-push apos a troca. Estado das estruturas pode estar comprometido.\n");
    }
}

/**
 * @brief Alterna as 3 primeiras pecas da fila com as 3 pecas da pilha.
 */
void trocarPecasMultiplaAcao(Fila *f, Pilha *p) {
    const int NUM_TROCA = 3;
    
    if (f->tamanho_atual < NUM_TROCA) {
        printf("\n❌ ERRO: A Fila deve ter pelo menos %d pecas. (Atual: %d)\n", NUM_TROCA, f->tamanho_atual);
        return;
    }
    if (p->topo + 1 < NUM_TROCA) {
        printf("\n❌ ERRO: A Pilha deve ter pelo menos %d pecas. (Atual: %d)\n", NUM_TROCA, p->topo + 1);
        return;
    }

    Peca temp_pilha[NUM_TROCA];
    Peca temp_fila[NUM_TROCA];
    
    printf("\n>>> 5. TROCA MULTIPLA INICIADA (3x3) <<<\n");

    // 1. Guarda as pecas da pilha (pop) e da fila
    for (int i = 0; i < NUM_TROCA; i++) {
        // Guarda as pecas da fila (indices da fila circular)
        int idx_fila = (f->inicio + i) % CAP_FILA;
        temp_fila[i] = f->elementos[idx_fila];
        
        // Guarda as pecas da pilha (pop)
        temp_pilha[NUM_TROCA - 1 - i] = pop(p); // Inverte a ordem para manter a pilha correta
    }

    // 2. Coloca as peças da pilha (guardadas) na frente da fila
    for (int i = 0; i < NUM_TROCA; i++) {
        int idx_fila = (f->inicio + i) % CAP_FILA;
        f->elementos[idx_fila] = temp_pilha[i];
    }

    // 3. Coloca as peças da fila (guardadas) na pilha (push)
    for (int i = 0; i < NUM_TROCA; i++) {
        push(p, temp_fila[i]);
    }

    printf("✅ ACAO: Troca em BLOCO (3 pecas) realizada com sucesso.\n");
}


// --- 5. Menu Principal ---

void menuPrincipal(Fila *f, Pilha *p) {
    int escolha;
    
    do {
        exibirEstado(f, p);

        printf("\n================ OPCOES ESTRATEGICAS ================\n");
        printf("Codigo | Acao\n");
        printf("---------------------------------------------------\n");
        printf("  1    | Jogar peca da fila (Dequeue e Reabastecer)\n");
        printf("  2    | Reservar peca (Fila -> Pilha)\n");
        printf("  3    | Usar peca reservada (Pop Pilha)\n");
        printf("  4    | Trocar peca atual (Frente Fila <-> Topo Pilha)\n");
        printf("  5    | Trocar 3 pecas (3 Fila <-> 3 Pilha)\n");
        printf("  0    | Sair do Programa\n");
        printf("===================================================\n");
        printf("Escolha o Codigo da Acao: ");
        
        if (scanf("%d", &escolha) != 1) {
            printf("\n[ERRO] Entrada invalida. Por favor, digite um numero.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (escolha) {
            case 1: jogarPecaAcao(f); break;
            case 2: reservarPecaAcao(f, p); break;
            case 3: usarPecaReservadaAcao(p, f); break;
            case 4: trocarPecaUnicaAcao(f, p); break;
            case 5: trocarPecasMultiplaAcao(f, p); break;
            case 0: printf("\n👋 Gerenciador de Pecas Encerrado. Bom jogo!\n"); break;
            default: printf("\n[ALERTA] Opcao invalida. Tente novamente.\n"); break;
        }
        
    } while (escolha != 0);
}

// --- Funcao Principal ---

int main() {
    Fila fila_pecas;
    Pilha pilha_reserva;

    inicializarFila(&fila_pecas);
    inicializarPilha(&pilha_reserva);

    // 1. Inicializar a fila de pecas com um numero fixo de elementos (CAP_FILA)
    printf("Iniciando Gerenciador de Pecas: Preenchendo Fila Inicial...\n");
    while (!fila_estaCheia(&fila_pecas)) {
        Peca p = gerarPeca();
        enqueue(&fila_pecas, p);
    }
    printf("Fila inicial preenchida com %d pecas.\n", CAP_FILA);
    
    // Inicia o menu de acoes
    menuPrincipal(&fila_pecas, &pilha_reserva);

    return 0;
}