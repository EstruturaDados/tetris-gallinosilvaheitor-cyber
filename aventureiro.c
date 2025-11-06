#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único
} Peca;

// ---------- FILA CIRCULAR ----------
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ---------- PILHA ----------
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

// ---------- FUNÇÕES DA FILA ----------
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

int filaCheia(Fila *fila) {
    return fila->quantidade == TAM_FILA;
}

int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

void enfileirar(Fila *fila, Peca peca) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->elementos[fila->tras] = peca;
    fila->quantidade++;
}

Peca desenfileirar(Fila *fila) {
    Peca removida = {'-', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->quantidade--;
    return removida;
}

// ---------- FUNÇÕES DA PILHA ----------
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("⚠️ Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
}

Peca desempilhar(Pilha *pilha) {
    Peca removida = {'-', -1};
    if (pilhaVazia(pilha)) {
        printf("⚠️ Nenhuma peça reservada.\n");
        return removida;
    }
    removida = pilha->elementos[pilha->topo];
    pilha->topo--;
    return removida;
}

// ---------- GERAÇÃO DE PEÇAS ----------
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // Escolhe um tipo aleatório
    nova.id = id;
    return nova;
}

// ---------- EXIBIÇÃO DO ESTADO ----------
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n=========================\n");
    printf("ESTADO ATUAL:\n");

    printf("Fila de peças: ");
    int i = fila->frente;
    for (int c = 0; c < fila->quantidade; c++) {
        Peca p = fila->elementos[i];
        printf("[%c %d] ", p.nome, p.id);
        i = (i + 1) % TAM_FILA;
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    for (int j = pilha->topo; j >= 0; j--) {
        Peca p = pilha->elementos[j];
        printf("[%c %d] ", p.nome, p.id);
    }
    printf("\n=========================\n");
}

// ---------- MENU PRINCIPAL ----------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idAtual = 0;
    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // JOGAR PEÇA
            if (!filaVazia(&fila)) {
                Peca jogada = desenfileirar(&fila);
                printf("Você jogou a peça [%c %d]\n", jogada.nome, jogada.id);
                enfileirar(&fila, gerarPeca(idAtual++));
            }
        } 
        else if (opcao == 2) {
            // RESERVAR PEÇA
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                Peca reservada = desenfileirar(&fila);
                empilhar(&pilha, reservada);
                printf("Peça [%c %d] movida para a reserva.\n", reservada.nome, reservada.id);
                enfileirar(&fila, gerarPeca(idAtual++));
            }
        } 
        else if (opcao == 3) {
            // USAR PEÇA RESERVADA
            Peca usada = desempilhar(&pilha);
            if (usada.id != -1)
                printf("Você usou a peça reservada [%c %d]\n", usada.nome, usada.id);
        } 
        else if (opcao != 0) {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    printf("\nEncerrando o jogo. Até logo!\n");
    return 0;
}
