#include <stdio.h>
#include <string.h>

#define MAX_ERROS 4

/* Retorna 0 no fim da entrada e rejeita linhas que nao cabem no buffer. */
static int ler_linha(char *destino, size_t tamanho) {
    for (;;) {
        int c;
        size_t len;
        if (fgets(destino, (int)tamanho, stdin) == NULL) return 0;
        len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
            return 1;
        }
        c = getchar();
        if (c == '\n' || c == EOF) return 1;
        while ((c = getchar()) != '\n' && c != EOF) {}
        puts("Entrada longa demais. Tente novamente:");
    }
}

static int ler_texto(const char *pedido, char *destino, size_t tamanho) {
    do {
        puts(pedido);
        if (!ler_linha(destino, tamanho)) return 0;
        if (destino[0] != '\0') return 1;
        puts("A entrada nao pode ficar vazia.");
    } while (1);
}

static int ler_opcao(const char *pedido) {
    char entrada[32];
    for (;;) {
        puts(pedido);
        if (!ler_linha(entrada, sizeof entrada)) return 0;
        if (strcmp(entrada, "1") == 0) return 1;
        if (strcmp(entrada, "2") == 0) return 2;
        puts("Opcao invalida. Digite 1 ou 2.");
    }
}

static char minuscula_ascii(char c) {
    return c >= 'A' && c <= 'Z' ? (char)(c + ('a' - 'A')) : c;
}

static int letra_ascii(char c) {
    return c >= 'a' && c <= 'z';
}

static int ler_palavra(char *palavra, size_t tamanho) {
    for (;;) {
        size_t i;
        int tem_letra = 0;
        if (!ler_texto("Digite a palavra ou frase (letras sem acento e espacos):", palavra, tamanho)) return 0;
        for (i = 0; palavra[i] != '\0'; i++) {
            palavra[i] = minuscula_ascii(palavra[i]);
            if (letra_ascii(palavra[i])) tem_letra = 1;
            else if (palavra[i] != ' ') break;
        }
        if (palavra[i] == '\0' && tem_letra) return 1;
        puts("Use letras de A a Z e espacos, sem acentos; inclua ao menos uma letra.");
    }
}

static void boneco(int erros) {
    static const char *desenhos[] = {
        " ___\n/  |\n|\n|\n|\n|_____",
        " ___\n/  |\n|  O\n|\n|\n|_____",
        " ___\n/  |\n|  O\n|  |\n|\n|_____",
        " ___\n/  |\n|  O\n| /|\\\n|\n|_____",
        " ___\n/  |\n|  O\n| /|\\\n| / \\\n|_____"
    };
    puts(desenhos[erros]);
}

int main(void) {
    char jogadores[2][10], tema[30], palavra[20], revelada[20];
    int pontos[2] = {0, 0};
    int opcao;

    puts("Bem-vindo ao jogo da forca! O adivinhador pode errar 4 vezes.");
    if (!ler_texto("Nome do jogador 1 (ate 9 caracteres):", jogadores[0], sizeof jogadores[0]) ||
        !ler_texto("Nome do jogador 2 (ate 9 caracteres):", jogadores[1], sizeof jogadores[1])) return 0;

    do {
        int adivinhador, desafiante, erros = 0;
        int tentadas[26] = {0};
        size_t i, tamanho;
        char entrada[32];

        printf("Pontos: %s %d x %d %s\n", jogadores[0], pontos[0], pontos[1], jogadores[1]);
        printf("Quem vai adivinhar? 1. %s  2. %s\n", jogadores[0], jogadores[1]);
        opcao = ler_opcao("Digite 1 ou 2:");
        if (!opcao) return 0;
        adivinhador = opcao - 1;
        desafiante = 1 - adivinhador;
        printf("%s, escolha o tema e a palavra. O outro jogador deve desviar o olhar.\n", jogadores[desafiante]);
        if (!ler_texto("Tema (ate 29 caracteres):", tema, sizeof tema) ||
            !ler_palavra(palavra, sizeof palavra)) return 0;

        /* Afasta a palavra da tela sem depender de comandos do sistema. */
        for (i = 0; i < 40; i++) putchar('\n');
        tamanho = strlen(palavra);
        for (i = 0; i < tamanho; i++) revelada[i] = palavra[i] == ' ' ? ' ' : '_';
        revelada[tamanho] = '\0';
        printf("Tema: %s. A palavra ou frase tem %zu caracteres, incluindo espacos.\n", tema, tamanho);
        boneco(erros);
        while (erros < MAX_ERROS && strcmp(revelada, palavra) != 0) {
            char tentativa;
            int encontrou = 0;
            printf("Palavra: %s\nDigite uma letra: ", revelada);
            if (!ler_linha(entrada, sizeof entrada)) return 0;
            tentativa = minuscula_ascii(entrada[0]);
            if (entrada[0] == '\0' || entrada[1] != '\0' || !letra_ascii(tentativa)) {
                puts("Digite uma unica letra de A a Z, sem acento.");
                continue;
            }
            if (tentadas[tentativa - 'a']) {
                puts("Voce ja tentou essa letra.");
                continue;
            }
            tentadas[tentativa - 'a'] = 1;
            for (i = 0; i < tamanho; i++) {
                if (palavra[i] == tentativa) {
                    revelada[i] = tentativa;
                    encontrou = 1;
                }
            }
            if (encontrou) puts("Letra correta!");
            else printf("Letra incorreta! Restam %d erro(s).\n", MAX_ERROS - ++erros);
            boneco(erros);
        }
        printf("Palavra: %s\n", palavra);
        if (strcmp(revelada, palavra) == 0) {
            puts("Voce ganhou a rodada!");
            pontos[adivinhador]++;
        } else {
            puts("Voce perdeu a rodada!");
            pontos[desafiante]++;
        }
        opcao = ler_opcao("Digite 1 para encerrar ou 2 para jogar novamente:");
        if (!opcao) return 0;
    } while (opcao == 2);
    printf("Obrigado por jogar! Pontos: %s %d x %d %s\n", jogadores[0], pontos[0], pontos[1], jogadores[1]);
    return 0;
}

