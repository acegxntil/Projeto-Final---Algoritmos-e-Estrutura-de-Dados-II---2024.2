#ifndef BST_H
#define BST_H

#define MAX_OFFSETS 1000  // Máximo de posições por palavra

// Nó da árvore binária de busca
typedef struct BSTNode {
    char word[100];                    // Palavra
    long offsets[MAX_OFFSETS];        // Posições no arquivo
    int numOffsets;                   // Quantidade de posições
    int frequency;                    // Frequência da palavra
    struct BSTNode *left, *right;     // Filhos
} BSTNode;

// Estrutura do repositório com a raiz da árvore
typedef struct {
    BSTNode *root;
} BSTRepo;

// Inicializa o repositório
void initBSTRepo(BSTRepo *repo);

// Reseta e libera a árvore
void resetBSTRepo(BSTRepo *repo);
void freeBSTRepo(BSTNode *root);

// Cria um novo nó
BSTNode* createBSTNode(const char *word, long offset);

// Insere ou atualiza uma palavra na árvore
void insertWordBST(BSTRepo *repo, const char *word, long offset);

// Busca uma palavra na árvore
BSTNode* findWordBST(BSTNode *root, const char *word);

#endif // BST_H
