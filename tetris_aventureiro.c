#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Struct que representa uma peça
typedef struct {
    int id;
    char tipo; // 'I', 'O', 'T', 'L'
} Peca;

// Fila circular
typedef struct {
    Peca fila[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Pilha linear
typedef struct {
    Peca pilha[TAM_PILHA];
    int topo;
} Pilha;

// --- Protótipos ---
void inicializarFila(Fila *f);
void mostrarFila(Fila f);
void mostrarPilha(Pilha p);
Peca gerarPeca();

int enfileirar(Fila *f, Peca p);
int desenfileirar(Fila *f, Peca *p);

void inicializarPilha(Pilha *p);
int empilhar(Pilha *p, Peca p);
int desempilhar(Pilha *p, Peca *p);

// --- Função principal ---
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    Peca pecaTemp;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        printf("\n===== TETRIS STACK - Nível Aventureiro =====\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: // Jogar peça
                if (desenfileirar(&fila, &pecaTemp)) {
                    printf("🎮 Peça jogada: ID=%d, Tipo=%c\n", pecaTemp.id, pecaTemp.tipo);
                    enfileirar(&fila, gerarPeca()); // Reposição automática
                } else {
                    printf("⚠️  Fila vazia.\n");
                }
                break;

            case 2: // Reservar peça
                if (desenfileirar(&fila, &pecaTemp)) {
                    if (empilhar(&pilha, pecaTemp)) {
                        printf("📦 Peça reservada: ID=%d, Tipo=%c\n", pecaTemp.id, pecaTemp.tipo);
                        enfileirar(&fila, gerarPeca()); // Reposição automática
                    } else {
                        printf("⚠️  Pilha cheia! Não foi possível reservar.\n");
                        // Recoloca a peça na frente da fila (rollback)
                        fila.frente = (fila.frente - 1 + TAM_FILA) % TAM_FILA;
                        fila.fila[fila.frente] = pecaTemp;
                        fila.quantidade++;
                    }
                } else {
                    printf("⚠️  Fila vazia. Nada para reservar.\n");
                }
                break;

            case 3: // Usar peça reservada
                if (desempilhar(&pilha, &pecaTemp)) {
                    printf("🎯 Peça usada da reserva: ID=%d, Tipo=%c\n", pecaTemp.id, pecaTemp.tipo);
                } else {
                    printf("⚠️  Pilha vazia. Nenhuma peça reservada.\n");
                }
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

        // Mostrar estado atual
        mostrarFila(fila);
        mostrarPilha(pilha);

    } while(opcao != 0);

    return 0;
}

// --- Implementações ---

// Inicializa a fila com 5 peças
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->quantidade = 0;

    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(f, gerarPeca());
    }
}

// Gera uma nova peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.id = rand() % 1000;
    p.tipo = tipos[rand() % 4];
    return p;
}

// Enfileira uma peça
int enfileirar(Fila *f, Peca p) {
    if (f->quantidade == TAM_FILA)
        return 0;

    f->fila[f->tras] = p;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->quantidade++;
    return 1;
}

// Desenfileira uma peça
int desenfileirar(Fila *f, Peca *p) {
    if (f->quantidade == 0)
        return 0;

    *p = f->fila[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return 1;
}

// Mostra a fila atual
void mostrarFila(Fila f) {
    printf("\n🧱 Fila Atual:\n");
    if (f.quantidade == 0) {
        printf("[vazia]\n");
        return;
    }

    int i, index;
    for (i = 0, index = f.frente; i < f.quantidade; i++, index = (index + 1) % TAM_FILA) {
        printf("[%d] ID=%d, Tipo=%c\n", i + 1, f.fila[index].id, f.fila[index].tipo);
    }
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Empilha uma peça
int empilhar(Pilha *p, Peca peca) {
    if (p->topo == TAM_PILHA - 1)
        return 0;

    p->topo++;
    p->pilha[p->topo] = peca;
    return 1;
}

// Desempilha uma peça
int desempilhar(Pilha *p, Peca *peca) {
    if (p->topo == -1)
        return 0;

    *peca = p->pilha[p->topo];
    p->topo--;
    return 1;
}

// Mostra a pilha atual
void mostrarPilha(Pilha p) {
    printf("\n🎒 Pilha de Reserva:\n");
    if (p.topo == -1) {
        printf("[vazia]\n");
        return;
    }

    for (int i = p.topo; i >= 0; i--) {
        printf("[%d] ID=%d, Tipo=%c\n", i + 1, p.pilha[i].id, p.pilha[i].tipo);
    }
}