#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// ----- Estado -----
typedef struct {
    Fila fila;
    Pilha pilha;
    int valido; // 1 se o estado é válido, 0 caso contrário
} Estado;

// ----- Controle global -----
int contadorID = 0;
char tipos[] = {'I', 'O', 'T', 'L'};

// ----- Gerar peça automaticamente -----
Peca gerarPeca() {
    Peca p = {tipos[rand()%4], contadorID++};
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
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        return 0;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;

    return 1;
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

int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        return 0;
    }

    p->itens[++p->topo] = x;
    return 1;
}

Peca pop(Pilha *p) {
    Peca vazio = {'-', -1};

    if (pilhaVazia(p)) {
        return vazio;
    }

    return p->itens[p->topo--];
}

// ===== ESTADO (UNDO) =====
void salvarEstado(Estado *e, Fila *f, Pilha *p) {
    e->fila = *f;
    e->pilha = *p;
    e->valido = 1;
}

void restaurarEstado(Estado *e, Fila *f, Pilha *p) {
    if (!e->valido) {
        printf("Nada para desfazer!\n");
        return;
    }

    *f = e->fila;
    *p = e->pilha;
    e->valido = 0; // só pode restaurar uma vez.
    printf("Acao desfeita!\n");
}

// ===== TROCAS =====
int trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        return 0;
    }

    int i = f->inicio;
    Peca temp = f->itens[i];
    f->itens[i] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    return 1;
}

int trocarTres(Fila *f, Pilha *p) {
    if (p->topo != 2 || f->tamanho < 3) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        int fi = (f->inicio + i) % TAM_FILA;
        Peca temp = f->itens[fi];
        f->itens[fi] = p->itens[2 - i];
        p->itens[2 - i] = temp;
    }

    return 1;
}


// ===== INVERTER =====
int inverter(Fila *f, Pilha *p) {
    if (p->topo != 2 || f->tamanho < 3) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        int fi = (f->inicio + i) % TAM_FILA;
        Peca temp = f->itens[fi];
        f->itens[fi] = p->itens[i]; // base -> topo
        p->itens[i] = temp;
    }

    return 1;
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
    Fila fila;
    Pilha pilha;
    Estado ultimo = {0};
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\n1 - Jogar peça (remover da frente)");
        printf("\n2 - Enviar peça da fila para a reserva (pilha)\n");
        printf("\n3 - Usar peça da reserva (remover do topo da pilha)\n");
        printf("\n4 - Trocar peça da frente com topo da pilha\n");
        printf("\n5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("\n6 - Desfazer última ação\n");
        printf("\n7 - Inverter os 3 primeiros da fila com os 3 da pilha\n");
        printf("\n0 - Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // limpar entrada
            opcao = -1; // opção inválida
        }

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    salvarEstado(&ultimo, &fila, &pilha);
                    Peca p = dequeue(&fila);
                    printf("Jogou [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                } else printf("Fila vazia!\n");
                break;
            }

            case 2: { // Reservar peça
                if (!pilhaCheia(&pilha) && !filaVazia(&fila)) {
                    salvarEstado(&ultimo, &fila, &pilha);
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    printf("Reservou [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                } else printf("Nao foi possivel reservar!\n");
                break;
            }

            case 3: { // Usar peça da reserva
                if (!pilhaVazia(&pilha)) {
                    salvarEstado(&ultimo, &fila, &pilha);
                    Peca p = pop(&pilha);
                    printf("Usou [%c %d]\n", p.nome, p.id);
                } else printf("Pilha vazia!\n");
                break;
            }

            case 4: // Trocar topo
                if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                    salvarEstado(&ultimo, &fila, &pilha);
                    trocarTopo(&fila, &pilha);
                    printf("Troca realizada!\n");
                } else printf("Nao pode trocar!\n");
                break;

            case 5: // Trocar 3
                if (pilha.topo == 2 && fila.tamanho >= 3) {
                    salvarEstado(&ultimo, &fila, &pilha);
                    trocarTres(&fila, &pilha);
                    printf("Troca dos 3!\n");
                } else printf("Nao pode trocar!\n");
                break;
            
            case 7: // Inverter
               if (pilha.topo == 2 && fila.tamanho >= 3) {
                salvarEstado(&ultimo, &fila, &pilha);
                inverter(&fila, &pilha);
                printf("Invertido!\n");
               } else printf("Nao pode inverter!\n");
               break;

            case 6: // Restaurar estado
                restaurarEstado(&ultimo, &fila, &pilha);
                break;            

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
