#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#define MAX_OFFSETS 1000   // Máximo de posições por palavra
#define MAX_WORDS 10000    // Máximo de palavras no repositório

// Palavra armazenada no vetor
typedef struct {
    char word[100];
    long offsets[MAX_OFFSETS];
    int numOffsets;
    int frequency;
} BSWord;

// Repositório com vetor de palavras ordenadas
typedef struct {
    BSWord words[MAX_WORDS];
    int numWords;
} BinarySearchRepo;

// Inicializa o repositório
void initBinarySearchRepo(BinarySearchRepo *repo);

// Limpa o repositório
void resetBinarySearchRepo(BinarySearchRepo *repo);

// Libera recursos do repositório
void freeBinarySearchRepo(BinarySearchRepo *repo);

// Insere ou atualiza uma palavra no repositório
void insertWordBS(BinarySearchRepo *repo, const char *word, long offset);

// Busca binária por uma palavra
BSWord* findWordBS(BinarySearchRepo *repo, const char *word);

#endif // BINARY_SEARCH_H
