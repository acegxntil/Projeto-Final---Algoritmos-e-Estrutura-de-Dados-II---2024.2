#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_search.h"

// Inicializa o repositório
void initBinarySearchRepo(BinarySearchRepo *repo) {
    repo->numWords = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        repo->words[i].numOffsets = 0;
        repo->words[i].frequency = 0;
        memset(repo->words[i].word, 0, sizeof(repo->words[i].word));
    }
}

// Insere nova palavra ou adiciona um novo registro ao final
void insertWordBS(BinarySearchRepo *repo, const char *word, long offset) {
    if (repo->numWords < MAX_WORDS) {
        BSWord *wordEntry = &repo->words[repo->numWords];
        strcpy(wordEntry->word, word);
        wordEntry->offsets[wordEntry->numOffsets++] = offset;
        wordEntry->frequency++;
        repo->numWords++;
    } else {
        printf("Limite de palavras atingido no repositório.\n");
    }
}

// Busca sequencial por uma palavra (poderia ser binária com vetor ordenado)
BSWord* findWordBS(BinarySearchRepo *repo, const char *word) {
    for (int i = 0; i < repo->numWords; i++) {
        if (strcmp(repo->words[i].word, word) == 0) {
            return &repo->words[i];
        }
    }
    return NULL;
}

// Reseta o repositório (zera os dados)
void resetBinarySearchRepo(BinarySearchRepo *repo) {
    repo->numWords = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        repo->words[i].numOffsets = 0;
        repo->words[i].frequency = 0;
        memset(repo->words[i].word, 0, sizeof(repo->words[i].word));
    }
}

// Libera (reseta) os dados do repositório
void freeBinarySearchRepo(BinarySearchRepo *repo) {
    repo->numWords = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        repo->words[i].numOffsets = 0;
        repo->words[i].frequency = 0;
        memset(repo->words[i].word, 0, sizeof(repo->words[i].word));
    }
}
