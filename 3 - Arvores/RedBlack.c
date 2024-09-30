#include <stdio.h>
#include <stdlib.h>

// Definindo as cores dos nós
#define VERMELHO 1
#define PRETO 0

// Estrutura para um nó da árvore
struct No {
    int valor;
    int cor; // 1 para vermelho, 0 para preto
    struct No *esquerda, *direita, *pai;
};

// Função para criar um novo nó
struct No* criarNo(int valor) {
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    novoNo->valor = valor;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->pai = NULL;
    novoNo->cor = VERMELHO; // Nós recém-inseridos são vermelhos
    return novoNo;
}

// Função para rotacionar à esquerda
void rotacionarEsquerda(struct No** raiz, struct No* x) {
    struct No* y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL)
        y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->esquerda)
        x->pai->esquerda = y;
    else
        x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
}

// Função para rotacionar à direita
void rotacionarDireita(struct No** raiz, struct No* y) {
    struct No* x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != NULL)
        x->direita->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL)
        *raiz = x;
    else if (y == y->pai->esquerda)
        y->pai->esquerda = x;
    else
        y->pai->direita = x;
    x->direita = y;
    y->pai = x;
}

// Função para corrigir as propriedades da árvore Red-Black após a inserção
void corrigirInsercao(struct No** raiz, struct No* z) {
    while (z != *raiz && z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            struct No* tio = z->pai->pai->direita;
            if (tio != NULL && tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacionarEsquerda(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarDireita(raiz, z->pai->pai);
            }
        } else {
            struct No* tio = z->pai->pai->esquerda;
            if (tio != NULL && tio->cor == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacionarDireita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacionarEsquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

// Função para inserir um novo nó na árvore Red-Black
void inserir(struct No** raiz, int valor) {
    struct No* novoNo = criarNo(valor);
    struct No* y = NULL;
    struct No* x = *raiz;

    // Inserção de um novo nó na árvore binária de busca
    while (x != NULL) {
        y = x;
        if (novoNo->valor < x->valor)
            x = x->esquerda;
        else
            x = x->direita;
    }

    novoNo->pai = y;
    if (y == NULL)
        *raiz = novoNo; // A árvore estava vazia
    else if (novoNo->valor < y->valor)
        y->esquerda = novoNo;
    else
        y->direita = novoNo;

    // Corrige as propriedades Red-Black da árvore
    corrigirInsercao(raiz, novoNo);
}

// Função para percorrer a árvore em ordem (inorder traversal)
void percorrerEmOrdem(struct No* raiz) {
    if (raiz != NULL) {
        percorrerEmOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        percorrerEmOrdem(raiz->direita);
    }
}

// Função principal
int main() {
    struct No* raiz = NULL;

    // Inserindo valores na árvore Red-Black
    inserir(&raiz, 10);
    inserir(&raiz, 20);
    inserir(&raiz, 30);
    inserir(&raiz, 15);
    inserir(&raiz, 25);

    // Percorrendo e imprimindo a árvore
    printf("Árvore Red-Black em ordem: ");
    percorrerEmOrdem(raiz);
    printf("\n");

    return 0;
}
