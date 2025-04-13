#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "binary_search.h"
#include "bst.h"
#include "avl.h"

#define MAX_LINE_SIZE 1024
#define MIN_WORD_LENGTH 4

// Função para converter uma string para minúsculas
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Função para extrair palavras de uma linha e inseri-las nos repositórios
void extractWords(char *line, long offset, BinarySearchRepo *bsRepo, BSTRepo *bstRepo, AVLRepo *avlRepo) {
    char *token;
    char *saveptr;
    char *delimiters = " \t\n\r.,;:!?\"'()[]{}";
    
    char *lineCopy = strdup(line); // Usar strdup para alocação de memória segura
    if (!lineCopy) {
        perror("Falha na alocação de memória"); // Usar perror para mensagens de erro
        return;
    }
    
    token = strtok_r(lineCopy, delimiters, &saveptr);
    while (token != NULL) {
        toLowerCase(token);
        if (strlen(token) >= MIN_WORD_LENGTH) {
            if (bsRepo) insertWordBS(bsRepo, token, offset);
            if (bstRepo) insertWordBST(bstRepo, token, offset);
            if (avlRepo) insertWordAVL(avlRepo, token, offset);
        }
        token = strtok_r(NULL, delimiters, &saveptr);
    }
    
    free(lineCopy); // Liberar a memória alocada
}

// Função para carregar citações de um arquivo e popular os repositórios
void loadQuotes(const char *filename, BinarySearchRepo *bsRepo, BSTRepo *bstRepo, AVLRepo *avlRepo) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo"); // Usar perror para mensagens de erro
        return;
    }
    
    char line[MAX_LINE_SIZE];
    long offset;
    clock_t startBS, endBS, startBST, endBST, startAVL, endAVL;
    
    resetBinarySearchRepo(bsRepo);
    resetBSTRepo(bstRepo);
    resetAVLRepo(avlRepo);

    // Busca Binária
    startBS = clock();
    while ((offset = ftell(file)), fgets(line, MAX_LINE_SIZE, file) != NULL) {
        extractWords(line, offset, bsRepo, NULL, NULL);
    }
    endBS = clock();
    rewind(file);

    // Árvore de Busca Binária (BST)
    startBST = clock();
    while ((offset = ftell(file)), fgets(line, MAX_LINE_SIZE, file) != NULL) {
        extractWords(line, offset, NULL, bstRepo, NULL);
    }
    endBST = clock();
    rewind(file);

    // Árvore AVL
    startAVL = clock();
    while ((offset = ftell(file)), fgets(line, MAX_LINE_SIZE, file) != NULL) {
        extractWords(line, offset, NULL, NULL, avlRepo);
    }
    endAVL = clock();

    fclose(file);

    printf("\nTempos de carregamento:\n");
    printf("Busca Binária: %.2f ms\n", 1000.0 * (endBS - startBS) / CLOCKS_PER_SEC);
    printf("Árvore de Busca Binária: %.2f ms\n", 1000.0 * (endBST - startBST) / CLOCKS_PER_SEC);
    printf("Árvore AVL: %.2f ms\n", 1000.0 * (endAVL - startAVL) / CLOCKS_PER_SEC);
}

// Função para buscar uma palavra nos repositórios
void searchWord(const char *word, const char *filename, BinarySearchRepo *bsRepo, BSTRepo *bstRepo, AVLRepo *avlRepo) {
    char wordLower[100];
    strcpy(wordLower, word);
    toLowerCase(wordLower);
    
    clock_t startBS, endBS, startBST, endBST, startAVL, endAVL;

    startBS = clock();
    BSWord *bsResult = findWordBS(bsRepo, wordLower);
    endBS = clock();

    startBST = clock();
    BSTNode *bstResult = findWordBST(bstRepo->root, wordLower);
    endBST = clock();

    startAVL = clock();
    AVLNode *avlResult = findWordAVL(avlRepo->root, wordLower);
    endAVL = clock();

    printf("\nTempos de busca:\n");
    printf("Busca Binária: %.2f ms\n", 1000.0 * (endBS - startBS) / CLOCKS_PER_SEC);
    printf("Árvore de Busca Binária: %.2f ms\n", 1000.0 * (endBST - startBST) / CLOCKS_PER_SEC);
    printf("Árvore AVL: %.2f ms\n", 1000.0 * (endAVL - startAVL) / CLOCKS_PER_SEC);

    printf("\nResultados para a palavra '%s':\n", wordLower);
    
    if (bsResult != NULL) {
        printf("Frequência (Busca Binária): %d\n", bsResult->frequency);
         if (bstResult != NULL) {
            printf("Frequência (BST): %d\n", bstResult->frequency);
        }
        if (avlResult != NULL) {
            printf("Frequência (AVL): %d\n", avlResult->frequency);
        }

        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("Erro ao abrir o arquivo para leitura de citações"); // Usar perror
            return;
        }

        char line[MAX_LINE_SIZE];
        char quote[MAX_LINE_SIZE];
        char movie[MAX_LINE_SIZE];
        char year[MAX_LINE_SIZE];

        for (int i = 0; i < bsResult->numOffsets; i++) {
            fseek(file, bsResult->offsets[i], SEEK_SET);
            if (fgets(line, MAX_LINE_SIZE, file) != NULL) {
                 if (sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"", quote, movie, year) == 3) {
                        printf("Citação: %s\nFilme: %s\nAno: %s\n\n", quote, movie, year);
                 }
                 else {
                        printf("Erro ao ler a linha: %s", line);
                 }
            }
        }

        fclose(file);
    } else {
        printf("Palavra não encontrada no repositório.\n");
    }
}

// Função para buscar palavras por frequência
void searchByFrequency(int frequency, AVLFrequencyRepo *freqRepo, const char *filename) {
    AVLFrequencyNode *result = findWordsByFrequencyAVL(freqRepo->root, frequency);
    
    if (result) {
        printf("\nPalavras com frequência %d:\n", frequency);
        printWordsWithFrequency(result, filename);
    } else {
        printf("\nNenhuma palavra encontrada com frequência %d.\n", frequency);
    }
}

// Função para construir a árvore AVL de frequência
AVLFrequencyRepo* buildFrequencyAVL(AVLRepo *wordRepo, const char *filename) {
    (void)filename; // Suprimir aviso de parâmetro não utilizado
    AVLFrequencyRepo *freqRepo = createAVLFrequencyRepo();
    buildFrequencyTreeFromWordTree(wordRepo->root, freqRepo);
    return freqRepo;
}

int main() {
    BinarySearchRepo *bsRepo = malloc(sizeof(BinarySearchRepo));
    if (bsRepo == NULL) {
        perror("Falha na alocação de memória para bsRepo");
        return 1;
    }
    initBinarySearchRepo(bsRepo);

    BSTRepo bstRepo;
    initBSTRepo(&bstRepo);

    AVLRepo avlRepo;
    initAVLRepo(&avlRepo);

    AVLFrequencyRepo *freqRepo = NULL;

    char filename[100] = "movie_quotes.csv";
    int choice;
    char searchWordBuffer[100];
    int searchFrequency;

    do {
        printf("\nRepositório de Citações de Filmes\n");
        printf("1. Carregar citações do arquivo\n");
        printf("2. Buscar uma palavra\n");
        printf("3. Buscar por frequência\n");
        printf("0. Sair\n");
        printf("Digite sua escolha: ");
        if (scanf("%d", &choice) != 1) {
            printf("Entrada inválida. Saindo.\n");
            break;
        }
        getchar(); // Consumir o caractere de nova linha

        switch (choice) {
            case 1:
                printf("Digite o nome do arquivo (padrão: movie_quotes.csv): ");
                if (fgets(filename, sizeof(filename), stdin) == NULL)
                {
                    perror("Erro ao ler o nome do arquivo");
                    choice = 0;
                    break;
                }
                size_t len = strlen(filename);
                if (len > 0 && filename[len - 1] == '\n') {
                    filename[len - 1] = '\0';
                }
                if (strlen(filename) == 0) {
                    strcpy(filename, "movie_quotes.csv");
                }

                printf("Carregando citações de %s...\n", filename);
                loadQuotes(filename, bsRepo, &bstRepo, &avlRepo);

                if (freqRepo != NULL) {
                    freeAVLFrequencyTree(freqRepo->root);
                    free(freqRepo);
                }

                freqRepo = buildFrequencyAVL(&avlRepo, filename);
                printf("Citações carregadas com sucesso!\n");
                break;

            case 2:
                printf("Digite a palavra para buscar: ");
                if (fgets(searchWordBuffer, sizeof(searchWordBuffer), stdin) == NULL) {
                    perror("Erro ao ler a palavra de busca");
                    choice = 0;
                    break;
                }
                len = strlen(searchWordBuffer);
                if (len > 0 && searchWordBuffer[len - 1] == '\n') {
                    searchWordBuffer[len - 1] = '\0';
                }

                searchWord(searchWordBuffer, filename, bsRepo, &bstRepo, &avlRepo);
                break;

            case 3:
                if (freqRepo == NULL) {
                    printf("Por favor, carregue as citações primeiro.\n");
                    break;
                }

                printf("Digite a frequência para buscar: ");
                if (scanf("%d", &searchFrequency) != 1) {
                        printf("Frequência inválida inserida.\n");
                        choice = 0;
                        break;
                }
                getchar(); // Consumir o caractere de nova linha
                searchByFrequency(searchFrequency, freqRepo, filename);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Escolha inválida. Por favor, tente novamente.\n");
        }
    } while (choice != 0);

    freeBinarySearchRepo(bsRepo);
    freeBSTRepo(bstRepo.root);
    freeAVLTree(avlRepo.root);

    if (freqRepo != NULL) {
        freeAVLFrequencyTree(freqRepo->root);
        free(freqRepo);
    }

    free(bsRepo); // Liberar a memória alocada para bsRepo

    return 0;
}