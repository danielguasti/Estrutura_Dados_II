#include <stdio.h>
#include <stdlib.h>

// AVL Tree Node structure
typedef struct noAVL {
    int data;           // Valor do nó
    struct noAVL *left;   // Ponteiro para o nó filho esquerdo
    struct noAVL *right;  // Ponteiro para o nó filho direito
    int height;         // Altura do nó para balanceamento
} noAVL;

// Instanciando as funções
noAVL* criaNo(int data);
int pegaAltura(noAVL *node);
int fatorBalanceamento(noAVL *node);
int max(int a, int b);
noAVL* rotacaoDireita(noAVL *y);
noAVL* rotacaoEsquerda(noAVL *x);
noAVL* insere(noAVL *node, int data);
noAVL* deleta(noAVL *root, int data);
noAVL* valorMinimo(noAVL *node);
void percorreEmOrdem(noAVL *root);
void printaArvore(noAVL *root, int space);

// Criando um novo nó
noAVL* criaNo(int data) {
    noAVL *newNode = (noAVL*)malloc(sizeof(noAVL));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;  // Novo nó inicialmente tem altura 1
    return newNode;
}

// Pega altura do nó
int pegaAltura(noAVL *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Calcula o fator de balanceamento do nó
int fatorBalanceamento(noAVL *node) {
    if (node == NULL)
        return 0;
    return pegaAltura(node->left) - pegaAltura(node->right);
}

// Pega o máximo de dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Faz uma rotação para a direita
noAVL* rotacaoDireita(noAVL *y) {
    noAVL *x = y->left;
    noAVL *T2 = x->right;

    // Faz a rotação
    x->right = y;
    y->left = T2;

    // Atualiza as alturas
    y->height = max(pegaAltura(y->left), pegaAltura(y->right)) + 1;
    x->height = max(pegaAltura(x->left), pegaAltura(x->right)) + 1;

    // Retorna nova raíz
    return x;
}

// Faz uma rotação para a esquerda
noAVL* rotacaoEsquerda(noAVL *x) {
    noAVL *y = x->right;
    noAVL *T2 = y->left;

    // Faz a rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    x->height = max(pegaAltura(x->left), pegaAltura(x->right)) + 1;
    y->height = max(pegaAltura(y->left), pegaAltura(y->right)) + 1;

    // Retorna nova raíz
    return y;
}

// Inser um nó na árvore AVL
noAVL* insere(noAVL *node, int data) {
    // Faz inserção igual da árvore binária
    if (node == NULL)
        return criaNo(data);

    if (data < node->data)
        node->left = insere(node->left, data);
    else if (data > node->data)
        node->right = insere(node->right, data);
    else // Valores iguais não são permitidos
        return node;

    // Atualiza altura do nó
    node->height = 1 + max(pegaAltura(node->left), pegaAltura(node->right));

    // Obtenha o fator de equilíbrio desse nó para verificar se esse nó ficou desequilibrado
    int balance = fatorBalanceamento
(node);

    // 4 rotações caso o nó esteja desbalanceado

    // Rotação esquerda 
    if (balance > 1 && data < node->left->data)
        return rotacaoDireita(node);

    // Rotação direita 
    if (balance < -1 && data > node->right->data)
        return rotacaoEsquerda(node);

    // Rotação dupla direita
    if (balance > 1 && data > node->left->data) {
        node->left = rotacaoEsquerda(node->left);
        return rotacaoDireita(node);
    }

    // Rotação dupla esquerda
    if (balance < -1 && data < node->right->data) {
        node->right = rotacaoDireita(node->right);
        return rotacaoEsquerda(node);
    }

    // Retorna o ponteiro do nó
    return node;
}

// Achar o valor mínimo
noAVL* valorMinimo(noAVL *node) {
    noAVL *current = node;
    // Loop para baixo da árvore para achar o menos valor
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Deleta um nó da árvore
noAVL* deleta(noAVL *root, int data) {
    // Deleta igual na árvore binária
    if (root == NULL)
        return root;

    // Se o valor a ser excluído for menor que o valor da raíz, então estará na esquerda da árvore
    if (data < root->data)
        root->left = deleta(root->left, data);

    // Se o valor a ser excluído for maior que o valor da raíz, então estará na direita da árvore
    else if (data > root->data)
        root->right = deleta(root->right, data);

    // Se o valor for igual o da raíz, então a raíz que deve ser excluída
    else {
        // Nó com apenas um filho ou sem filhos 
        if ((root->left == NULL) || (root->right == NULL)) {
            noAVL *temp = root->left ? root->left : root->right;

            // Sem filho
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else // Um filho
                *root = *temp; // Copia o conteúdo do filho não vazioo
            free(temp);
        }
        else {
            // Nó com dois filhos: Obtenha o sucessor emordem (menor na subárvore direita)
            noAVL *temp = valorMinimo(root->right);

            // Copie os dados do sucessor emordem para este nó
            root->data = temp->data;

            // Deleta o sucessor emordem
            root->right = deleta(root->right, temp->data);
        }
    }

    // Se a árvore só tiver um nó então retorna
    if (root == NULL)
        return root;

    // Atualiza altura do nó atual
    root->height = 1 + max(pegaAltura(root->left), pegaAltura(root->right));

    // Obtenha o fator de equilíbrio desse nó (para verificar se esse nó ficou desequilibrado)
    int balance = fatorBalanceamento
(root);

    // Se esse nó ficou desbalanceado, então existem 4 rotações

    // Rotação esquerda
    if (balance > 1 && fatorBalanceamento
(root->left) >= 0)
        return rotacaoDireita(root);

    // Rotação dupla direita
    if (balance > 1 && fatorBalanceamento
(root->left) < 0) {
        root->left = rotacaoEsquerda(root->left);
        return rotacaoDireita(root);
    }

    // Rotação direita
    if (balance < -1 && fatorBalanceamento
(root->right) <= 0)
        return rotacaoEsquerda(root);

    // Rotação dupla esquerda
    if (balance < -1 && fatorBalanceamento
(root->right) > 0) {
        root->right = rotacaoDireita(root->right);
        return rotacaoEsquerda(root);
    }

    return root;
}

// Percorre EmOrdem
void percorreEmOrdem(noAVL *root) {
    if (root != NULL) {
        percorreEmOrdem(root->left);
        printf("%d ", root->data);
        percorreEmOrdem(root->right);
    }
}

// Printa a estrutura da árvore
void printaArvore(noAVL *root, int space) {
    // Base case
    if (root == NULL)
        return;

    // Aumenta a distância entre os níveis
    space += 10;

    // Printa o filho direito primeiro
    printaArvore(root->right, space);

    // Imprimir o nó atual depois do espaço
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->data);

    // Printa o filho esquerdo
    printaArvore(root->left, space);
}


int main() {
    noAVL *root = NULL;

    // Insere nós
    root = insere(root, 30);
    root = insere(root, 24);
    root = insere(root, 20);

    printf("Árvore AVL inicial:\n");
    printaArvore(root, 0);

    printf("\nPercorre EmOrdem: ");
    percorreEmOrdem(root);
    printf("\n");

    // Insere um novo nó
    root = insere(root, 31);
    printf("\nDepois de inserir 31:\n");
    printaArvore(root, 0);

    // Apaga um nó
    root = deleta(root, 35);
    printf("\nDepois de deletar 35:\n");
    printaArvore(root, 0);

    return 0;
}
