#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.

#define TAM_FILA 5
#define TAM_PILHA 3

// ----- Estrutura da peça -----
typedef struct {
    char nome; // I, O, T, L
    int id;
} Peca;

// ----- Estrutura da fila circular -----
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// ----- Pilha -----
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ----- Controle global -----
int contadorID = 0;
char tipos[] = {'I', 'O', 'T', 'L'};

// ----- Gerar peça automaticamente -----
Peca gerarPeca() {
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// ----- inicializar fila -----
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

// ----- Verificações -----
int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// ----- Enqueue -----
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
}

// ----- Dequeue -----
Peca dequeue(Fila *f) {
    Peca vazio = {'-', -1};

    if (filaVazia(f)) {
        return vazio;
    }

    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;

    return p;
}

// ===== PILHA =====
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Não pode reservar.\n");
        return;
    }
    
    p->itens[++p->topo] = x;
}

Peca pop(Pilha *p) {
    Peca vazio = {'-', -1};

    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return vazio;
    }

    return p->itens[p->topo--];
}

// ----- EXIBIÇÃO -----
void exibirFila(Fila *f) {
    printf("\nFila de pecas:\n");

    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }

    int idx = f->inicio;

    for (int i = 0; i < f->tamanho; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha (Topo -> Base): ");

    if (pilhaVazia(p)) {
        printf("[vazia]\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}


int main() {

    // 🧩 Nível Novato: Fila de Peças Futuras
    //
    // - Crie uma struct Peca com os campos: tipo (char) e id (int).
    // - Implemente uma fila circular com capacidade para 5 peças.
    // - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
    // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
    // - Exiba a fila após cada ação com uma função mostrarFila().
    // - Use um menu com opções como:
    //      1 - Jogar peça (remover da frente)
    //      0 - Sair
    // - A cada remoção, insira uma nova peça ao final da fila.
    

    // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
    //
    // - Implemente uma pilha linear com capacidade para 3 peças.
    // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
    // - Permita enviar uma peça da fila para a pilha (reserva).
    // - Crie um menu com opção:
    //      2 - Enviar peça da fila para a reserva (pilha)
    //      3 - Usar peça da reserva (remover do topo da pilha)
    // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
    // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).
    Fila fila;
    Pilha pilha;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // preencher fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\n\n1 - Jogar peca");
        printf("\n2 - Reservar peca");
        printf("\n3 - Usar peca reservada");
        printf("\n0 - Sair");
        printf("\nOpcao: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1;
        }

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca p = dequeue(&fila);

                    if (p.id != -1) {
                        printf("Jogou: [%c %d]\n", p.nome, p.id);
                        enqueue(&fila, gerarPeca());
                    }
                }
                break;
            }

            case 2: {
                if (!pilhaCheia(&pilha) && !filaVazia(&fila)) {
                    Peca p = dequeue(&fila);
                    
                    if (p.id != -1) {
                        push(&pilha, p);
                        printf("Reservou: [%c %d]\n", p.nome, p.id);
                        enqueue(&fila, gerarPeca());
                    }
                } else {
                    printf("Nao foi possivel reservar!\n");
                }
                break;
            }

            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca p = pop(&pilha);
                    
                    if (p.id != -1) {
                        printf("Usou reserva: [%c %d]\n", p.nome, p.id);
                        enqueue(&fila, gerarPeca());
                    }
                } else {
                    printf("Nada na reserva!\n");
                }
                break;
            }

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);


    // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
    //
    // - Implemente interações avançadas entre as estruturas:
    //      4 - Trocar a peça da frente da fila com o topo da pilha
    //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
    // - Para a opção 4:
    //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
    //      Troque os elementos diretamente nos arrays.
    // - Para a opção 5:
    //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
    //      Use a lógica de índice circular para acessar os primeiros da fila.
    // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
    // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
    // - O menu deve ficar assim:
    //      4 - Trocar peça da frente com topo da pilha
    //      5 - Trocar 3 primeiros da fila com os 3 da pilha


    return 0;
}
