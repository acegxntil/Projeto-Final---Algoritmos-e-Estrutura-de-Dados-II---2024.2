#include <stdio.h>
#include <stdlib.h>

int main() {
    int escolha;

    printf ("Bem vindo ao Repositório de citações de filmes!\n"); // Inicia o menu para coletar a escolha do usuário

    printf("Escolha uma opção:\n");
    printf("1 - Carregar citações\n");
    printf("2 - Buscar palavra\n");
    printf("3 - Buscar por frequência\n");
    printf("4 - Sair\n");

    scanf("%d", &escolha);

    switch (escolha) {      //Realiza as opções escolhidas pelo usuário, caso não seja nenhuma das opções, retorna uma mensagem de erro
        case 1:
            printf("Digite o nome do arquivo:\n");
            break;
        case 2:
            printf("Digite a palavra para buscar:\n");
            break;
        case 3:
            printf("Buscar por frequência\n");
            break;
        case 4:
            printf("Sair\n");
            break;
        default:
            printf("Opção inválida\n");
            break;
    }

    return 0;
}