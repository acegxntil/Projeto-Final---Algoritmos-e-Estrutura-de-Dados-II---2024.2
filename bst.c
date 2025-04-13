#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

// Inicializa o repositório BST
void initBSTRepo(BSTRepo *repo) {
    repo->root = NULL;
}

// Libera a árvore e reseta o repositório
void resetBSTRepo(BSTRepo *repo) {
    freeBSTRepo(repo->root);
    repo->root = NULL;
}

// Libera todos os nós da árvore iterativamente
void freeBSTRepo(BSTNode *root) {
    if (root == NULL) return;

    BSTNode *stack[10000];
    int top = -1;

    stack[++top] = root;

    while (top >= 0) {
        BSTNode *node = stack[top--];

        if (node->left) stack[++top] = node->left;
        if (node->right) stack[++top] = node->right;

        free(node);
    }
}

// Cria um novo nó com a palavra e offset
BSTNode* createBSTNode(const char *word, long offset) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (!node) {
        printf("Falha na alocação de memória\n");
        return NULL;
    }

    strcpy(node->word, word);
    node->offsets[0] = offset;
    node->numOffsets = 1;
    node->frequency = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// Insere uma palavra na árvore BST
BSTNode* insertBSTNode(BSTNode *root, const char *word, long offset) {
    if (root == NULL) {
        return createBSTNode(word, offset);
    }

    int cmp = strcmp(word, root->word);

    if (cmp < 0) {
        root->left = insertBSTNode(root->left, word, offset);
    } else if (cmp > 0) {
        root->right = insertBSTNode(root->right, word, offset);
    } else {
        // Palavra já existe: atualiza frequência e adiciona offset
        root->frequency++;

        if (root->numOffsets < MAX_OFFSETS) {
            int offsetExists = 0;
            for (int i = 0; i < root->numOffsets; i++) {
                if (root->offsets[i] == offset) {
                    offsetExists = 1;
                    break;
                }
            }
            if (!offsetExists) {
                root->offsets[root->numOffsets++] = offset;
            }
        }
    }

    return root;
}

// Insere no repositório BST
void insertWordBST(BSTRepo *repo, const char *word, long offset) {
    if (repo == NULL) return;

    repo->root = insertBSTNode(repo->root, word, offset);
}

// Busca uma palavra na BST
BSTNode* findWordBST(BSTNode *root, const char *word) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(word, root->word);

    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return findWordBST(root->left, word);
    } else {
        return findWordBST(root->right, word);
    }
}
