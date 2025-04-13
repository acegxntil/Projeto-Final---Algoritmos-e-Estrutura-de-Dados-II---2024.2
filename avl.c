#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "avl.h"

// Função auxiliar para obter a altura de um nó AVL
int height(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

// Função auxiliar para obter o fator de balanceamento de um nó AVL
int getBalance(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// Função auxiliar para obter o maior entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Rotação simples para a direita (caso Esquerda-Esquerda)
AVLNode* rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Realiza a rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Retorna nova raiz
    return x;
}

// Rotação simples para a esquerda (caso Direita-Direita)
AVLNode* leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Realiza a rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Retorna nova raiz
    return y;
}

// Inicializa o repositório AVL
void initAVLRepo(AVLRepo *repo) {
    repo->root = NULL;
}

// Libera e reinicializa o repositório AVL
void resetAVLRepo(AVLRepo *repo) {
    freeAVLTree(repo->root);
    repo->root = NULL;
}

// Libera recursivamente a árvore AVL
void freeAVLTree(AVLNode *root) {
    if (root == NULL) return;

    freeAVLTree(root->left);
    freeAVLTree(root->right);
    free(root);
}

// Cria um novo nó AVL
AVLNode* createAVLNode(const char *word, long offset) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (!node) {
        printf("Erro de alocação de memória\n");
        return NULL;
    }

    strcpy(node->word, word);
    node->offsets[0] = offset;
    node->numOffsets = 1;
    node->frequency = 1;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// Insere uma palavra na árvore AVL
AVLNode* insertAVLNode(AVLNode *root, const char *word, long offset) {
    if (root == NULL) {
        return createAVLNode(word, offset);
    }

    int cmp = strcmp(word, root->word);

    if (cmp < 0) {
        root->left = insertAVLNode(root->left, word, offset);
    } else if (cmp > 0) {
        root->right = insertAVLNode(root->right, word, offset);
    } else {
        // Palavra já existe, atualiza frequência e offsets
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
        return root;
    }

    // Atualiza altura do nó
    root->height = 1 + max(height(root->left), height(root->right));

    // Calcula o balanceamento
    int balance = getBalance(root);

    // Casos de rotação
    if (balance > 1 && strcmp(word, root->left->word) < 0) {
        return rightRotate(root); // Esquerda-Esquerda
    }

    if (balance < -1 && strcmp(word, root->right->word) > 0) {
        return leftRotate(root); // Direita-Direita
    }

    if (balance > 1 && strcmp(word, root->left->word) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root); // Esquerda-Direita
    }

    if (balance < -1 && strcmp(word, root->right->word) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root); // Direita-Esquerda
    }

    return root;
}

// Função pública para inserir palavra no repositório
void insertWordAVL(AVLRepo *repo, const char *word, long offset) {
    if (repo == NULL) return;
    repo->root = insertAVLNode(repo->root, word, offset);
}

// Busca por palavra na árvore AVL
AVLNode* findWordAVL(AVLNode *root, const char *word) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(word, root->word);

    if (cmp == 0) return root;
    else if (cmp < 0) return findWordAVL(root->left, word);
    else return findWordAVL(root->right, word);
}

// Funções auxiliares para árvore AVL de frequência
int heightFreq(AVLFrequencyNode *node) {
    if (node == NULL) return 0;
    return node->height;
}

int getBalanceFreq(AVLFrequencyNode *node) {
    if (node == NULL) return 0;
    return heightFreq(node->left) - heightFreq(node->right);
}

AVLFrequencyNode* rightRotateFreq(AVLFrequencyNode *y) {
    AVLFrequencyNode *x = y->left;
    AVLFrequencyNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(heightFreq(y->left), heightFreq(y->right)) + 1;
    x->height = max(heightFreq(x->left), heightFreq(x->right)) + 1;

    return x;
}

AVLFrequencyNode* leftRotateFreq(AVLFrequencyNode *x) {
    AVLFrequencyNode *y = x->right;
    AVLFrequencyNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(heightFreq(x->left), heightFreq(x->right)) + 1;
    y->height = max(heightFreq(y->left), heightFreq(y->right)) + 1;

    return y;
}

// Cria um novo nó da árvore AVL de frequência
AVLFrequencyNode* createAVLFrequencyNode(int frequency) {
    AVLFrequencyNode *node = (AVLFrequencyNode *)malloc(sizeof(AVLFrequencyNode));
    if (!node) {
        printf("Erro de alocação de memória\n");
        return NULL;
    }

    node->frequency = frequency;
    node->words = NULL;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// Adiciona palavra na lista de um nó de frequência
void addWordToFrequencyNode(AVLFrequencyNode *node, const char *word) {
    WordListNode *newWord = (WordListNode *)malloc(sizeof(WordListNode));
    if (!newWord) {
        printf("Erro de alocação de memória\n");
        return;
    }

    strcpy(newWord->word, word);
    newWord->next = node->words;
    node->words = newWord;
}

// Insere um nó na árvore AVL de frequência
AVLFrequencyNode* insertAVLFrequencyNode(AVLFrequencyNode *root, int frequency, const char *word) {
    if (root == NULL) {
        AVLFrequencyNode *newNode = createAVLFrequencyNode(frequency);
        if (newNode) {
            addWordToFrequencyNode(newNode, word);
        }
        return newNode;
    }

    if (frequency < root->frequency) {
        root->left = insertAVLFrequencyNode(root->left, frequency, word);
    } else if (frequency > root->frequency) {
        root->right = insertAVLFrequencyNode(root->right, frequency, word);
    } else {
        addWordToFrequencyNode(root, word);
        return root;
    }

    root->height = 1 + max(heightFreq(root->left), heightFreq(root->right));

    int balance = getBalanceFreq(root);

    if (balance > 1 && frequency < root->left->frequency) return rightRotateFreq(root);
    if (balance < -1 && frequency > root->right->frequency) return leftRotateFreq(root);
    if (balance > 1 && frequency > root->left->frequency) {
        root->left = leftRotateFreq(root->left);
        return rightRotateFreq(root);
    }
    if (balance < -1 && frequency < root->right->frequency) {
        root->right = rightRotateFreq(root->right);
        return leftRotateFreq(root);
    }

    return root;
}

// Cria repositório AVL de frequência
AVLFrequencyRepo* createAVLFrequencyRepo() {
    AVLFrequencyRepo *repo = (AVLFrequencyRepo *)malloc(sizeof(AVLFrequencyRepo));
    if (!repo) {
        printf("Erro de alocação de memória\n");
        return NULL;
    }

    repo->root = NULL;
    return repo;
}

// Percorre árvore de palavras e constrói árvore de frequência
void buildFrequencyTreeHelper(AVLNode *wordNode, AVLFrequencyRepo *freqRepo) {
    if (wordNode == NULL) return;

    buildFrequencyTreeHelper(wordNode->left, freqRepo);
    freqRepo->root = insertAVLFrequencyNode(freqRepo->root, wordNode->frequency, wordNode->word);
    buildFrequencyTreeHelper(wordNode->right, freqRepo);
}

// Cria árvore de frequência a partir da árvore de palavras
void buildFrequencyTreeFromWordTree(AVLNode *wordRoot, AVLFrequencyRepo *freqRepo) {
    buildFrequencyTreeHelper(wordRoot, freqRepo);
}

// Busca por frequência na árvore AVL de frequência
AVLFrequencyNode* findWordsByFrequencyAVL(AVLFrequencyNode *root, int frequency) {
    if (root == NULL) return NULL;

    if (frequency == root->frequency) return root;
    else if (frequency < root->frequency) return findWordsByFrequencyAVL(root->left, frequency);
    else return findWordsByFrequencyAVL(root->right, frequency);
}

// Imprime todas as palavras com uma frequência específica
void printWordsWithFrequency(AVLFrequencyNode *node, const char *filename) {
    if (node == NULL || node->words == NULL) return;

    printf("Palavras com frequência %d:\n", node->frequency);
    WordListNode *current = node->words;
    while (current != NULL) {
        printf("- %s\n", current->word);

        FILE *file = fopen(filename, "r");
        if (file) {
            char line[1024];
            char quote[1024];
            char movie[1024];
            char year[1024];

            while (fgets(line, sizeof(line), file)) {
                if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"", quote, movie, year) == 3) {
                    char lowerQuote[1024];
                    strcpy(lowerQuote, quote);
                    for (int i = 0; lowerQuote[i]; i++) {
                        lowerQuote[i] = tolower(lowerQuote[i]);
                    }

                    char *wordInQuote = strstr(lowerQuote, current->word);
                    if (wordInQuote) {
                        printf("  Citação: %s\n  Filme: %s\n  Ano: %s\n\n", quote, movie, year);
                    }
                }
            }

            fclose(file);
        }

        current = current->next;
    }
}

// Libera a árvore AVL de frequência
void freeAVLFrequencyTree(AVLFrequencyNode *root) {
    if (root == NULL) return;

    freeAVLFrequencyTree(root->left);
    freeAVLFrequencyTree(root->right);

    WordListNode *current = root->words;
    while (current != NULL) {
        WordListNode *temp = current;
        current = current->next;
        free(temp);
    }

    free(root);
}
