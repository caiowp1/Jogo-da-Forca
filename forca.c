#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void boneco(int erros);

void limpar_buffer() {
    int c;
    // Lê e descarta caracteres até encontrar um '\n' ou o fim do arquivo (EOF)
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void)
{
    
    char jogador1[10];
    char jogador2[10];
    char advinhador[10];
    char palavra[20];
    char tentativa;
    int opcao = 0;
    int pontos1 = 0;
    int pontos2 = 0;
    int *pontosAdvinhador;
    int *pontosDesafiante; 
   
    printf("Bem vindo ao jogo da forca!\n");
    printf("O advinhador podera errar ate 4 vezes.\n");
    printf("Digite o nome do Jogador 1:\n");

    scanf("%s", jogador1);

    printf("Digite o nome do Jogador 2:\n");

    scanf("%s", jogador2);

    while (opcao != 1) {
      
        int acertos = 0;
        int erros = 0;

        printf("Pontos de %s: %d\n", jogador1, pontos1);
        printf("Pontos de %s: %d\n", jogador2, pontos2);

        printf("Quem advinhara a palavra secreta?\n");
        printf("%s ou %s\n", jogador1, jogador2);

        scanf("%s", advinhador);

        while ((strcmp(advinhador, jogador1) != 0) && (strcmp(advinhador, jogador2) != 0)) {

            printf("Jogador invalido. Selecione %s ou %s\n", jogador1, jogador2);

            scanf("%s", advinhador);

        } 
    
        if (strcmp(advinhador, jogador1) == 0) {

            pontosAdvinhador = &pontos1;
            pontosDesafiante = &pontos2;

            printf("%s, digite a palavra secreta:\n", jogador2);
            scanf("%s", palavra);

        } else if (strcmp(advinhador, jogador2) == 0) {

            pontosAdvinhador = &pontos2;
            pontosDesafiante = &pontos1;

            printf("%s, digite a palavra secreta:\n", jogador1);
            scanf("%s", palavra);

        }
        
        system("clear || cls");

        printf("A palavra secreta foi definida. Vamos comecar o jogo!\n");
        printf("A palavra secreta tem %lu letras.\n", strlen(palavra));

        int tamanho = strlen(palavra); 
        char letrasCertas[20]; 

        for (int i = 0; i < tamanho; i++) {
           // letrasCertas[i] = '_';

            if (palavra[i] == ' ') {
                letrasCertas[i] = palavra[i];
            } else {
                letrasCertas[i] = '_';
            }

        }
        letrasCertas[tamanho] = '\0';

        while ((erros < 4) && (strcmp(letrasCertas, palavra) != 0) && (acertos < tamanho)) {

            printf("Palavra: %s\n", letrasCertas);
            printf("Digite uma letra:\n");

            scanf(" %c", &tentativa);
            limpar_buffer();

            for (int i = 0; i < strlen(palavra); i++) {

                if (tentativa == palavra[i]) {

                    letrasCertas[i] = tentativa;

                } 

            }    

            if (strchr(letrasCertas, tentativa) != NULL) {

                acertos++;
                printf("Letra correta!\n");

            } else {

                erros++;
                printf("Letra incorreta! Voce pode errar mais %d vez(es).\n", 4 - erros);

            }

            boneco(erros);

        }

        printf("Palavra: %s\n", letrasCertas);

        if (strcmp(letrasCertas, palavra) == 0) {

            printf("Voce ganhou a rodada!\n");
            *pontosAdvinhador += 1;

        } else {

            printf("Voce perdeu! A palavra correta era %s\n", palavra);
            *pontosDesafiante += 1;

        }

        printf("Digite 1 para encerrar\n");
        printf("Digite 2 para jogar novamente\n");

        scanf(" %d", &opcao);

        while ((opcao != 1) && (opcao != 2)) {

            printf("Opcao invalida.\n");
            printf("Digite 1 para encerrar\n");
            printf("Digite 2 para jogar novamente\n");
            scanf(" %d", &opcao);

        }

        if (opcao == 2) {
            acertos = 0;
            erros = 0;
            system("clear || cls");
        } 

    }

    system("clear || cls");
    printf("Obrigado por jogar!\n");
    printf("Pontos de %s: %d\n", jogador1, pontos1);
    printf("Pontos de %s: %d\n", jogador2, pontos2);

    printf("pressione enter para encerrar...\n");
    limpar_buffer();
    getchar();     

}

void boneco(int erros) {
    
    switch(erros) {

        case 0:

        printf(" ___   \n");
        printf("/  |   \n");
        printf("|      \n");
        printf("|      \n");
        printf("|      \n");
        printf("|_____ \n");
        break;
        
        case 1:
        
        printf(" ___  \n");
        printf("/  |  \n");
        printf("|  O  \n");
        printf("|     \n");
        printf("|     \n");
        printf("|_____\n");
        break;
        
        case 2:
        
        printf(" ___  \n");
        printf("/  |  \n");
        printf("|  O  \n");
        printf("|  |  \n");
        printf("|     \n");
        printf("|_____\n");
        break;
        
        case 3: 
        
        printf(" ___   \n");
        printf("/  |   \n");
        printf("|  O   \n");
        printf("| /|\\ \n");
        printf("|      \n");
        printf("|_____ \n");
        break;
        
        case 4: 
        
        printf(" ___   \n");
        printf("/  |   \n");
        printf("|  O   \n");
        printf("| /|\\ \n");
        printf("| / \\ \n");
        printf("|_____ \n");
        break;
        
    }
    
}