#ifndef AVL_H
#define AVL_H

#define MAX_OFFSETS 1000  // Máximo de posições registradas por palavra

// Nó da árvore AVL de palavras
typedef struct AVLNode {
    char word[100];                    // Palavra
    long offsets[MAX_OFFSETS];        // Posições no arquivo
    int numOffsets;                   // Quantidade de posições
    int frequency;                    // Frequência da palavra
    int height;                       // Altura do nó
    struct AVLNode *left, *right;     // Filhos
} AVLNode;

// Repositório AVL de palavras
typedef struct {
    AVLNode *root;
} AVLRepo;

// Nó da lista de palavras (para nós com mesma frequência)
typedef struct WordListNode {
    char word[100];
    struct WordListNode *next;
} WordListNode;

// Nó da árvore AVL de frequências
typedef struct AVLFrequencyNode {
    int frequency;
    WordListNode *words;              // Lista de palavras com essa frequência
    int height;
    struct AVLFrequencyNode *left, *right;
} AVLFrequencyNode;

// Repositório AVL baseado em frequência
typedef struct {
    AVLFrequencyNode *root;
} AVLFrequencyRepo;

// Operações sobre AVL de palavras
void initAVLRepo(AVLRepo *repo);
void resetAVLRepo(AVLRepo *repo);
void freeAVLTree(AVLNode *root);
AVLNode* createAVLNode(const char *word, long offset);
void insertWordAVL(AVLRepo *repo, const char *word, long offset);
AVLNode* findWordAVL(AVLNode *root, const char *word);

// Operações sobre AVL de frequência
AVLFrequencyRepo* createAVLFrequencyRepo();
void buildFrequencyTreeFromWordTree(AVLNode *wordRoot, AVLFrequencyRepo *freqRepo);
AVLFrequencyNode* findWordsByFrequencyAVL(AVLFrequencyNode *root, int frequency);
void printWordsWithFrequency(AVLFrequencyNode *node, const char *filename);
void freeAVLFrequencyTree(AVLFrequencyNode *root);

#endif // AVL_H
