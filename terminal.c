#include "terminal.h"
#include <stdio.h>
#include <string.h>

static int ler_linha(char *destino, size_t tamanho) {
    for (;;) {
        size_t len;
        int c;
        if (!fgets(destino, (int)tamanho, stdin)) return 0;
        len = strlen(destino);
        if (len && destino[len - 1] == '\n') { destino[len - 1] = '\0'; return 1; }
        c = getchar();
        if (c == '\n' || c == EOF) return 1;
        while ((c = getchar()) != '\n' && c != EOF) {}
        puts("Entrada longa demais. Tente novamente:");
    }
}

int terminal_ler_texto(const char *pedido, char *destino, size_t tamanho) {
    for (;;) {
        size_t i;
        puts(pedido);
        if (!ler_linha(destino, tamanho)) return 0;
        for (i = 0; destino[i] == ' '; i++) {}
        if (destino[i] != '\0') return 1;
        puts("A entrada nao pode ficar vazia.");
    }
}

int terminal_ler_opcao(const char *pedido) {
    char entrada[32];
    for (;;) {
        puts(pedido);
        if (!ler_linha(entrada, sizeof entrada)) return 0;
        if (!strcmp(entrada, "1")) return 1;
        if (!strcmp(entrada, "2")) return 2;
        puts("Opcao invalida. Digite 1 ou 2.");
    }
}

int terminal_ler_letra(char *letra) {
    char entrada[32];
    for (;;) {
        printf("Digite uma letra: ");
        if (!ler_linha(entrada, sizeof entrada)) return 0;
        if (entrada[0] && !entrada[1] && ((entrada[0] >= 'a' && entrada[0] <= 'z') || (entrada[0] >= 'A' && entrada[0] <= 'Z'))) {
            *letra = entrada[0]; return 1;
        }
        puts("Digite uma unica letra de A a Z, sem acento.");
    }
}

void terminal_esconder_palavra(void) {
    int i;
    for (i = 0; i < 40; i++) putchar('\n');
}

void terminal_mostrar_jogo(const Jogo *jogo, const char *tema) {
    static const char *desenhos[] = {
        " ___\n/  |\n|\n|\n|\n|_____", " ___\n/  |\n|  O\n|\n|\n|_____",
        " ___\n/  |\n|  O\n|  |\n|\n|_____", " ___\n/  |\n|  O\n| /|\\\n|\n|_____",
        " ___\n/  |\n|  O\n| /|\\\n| / \\\n|_____"
    };
    size_t i;
    int alguma = 0;
    printf("\n--- TEMA: %s ---\n", tema);
    puts(desenhos[jogo->erros]);
    printf("Palavra: ");
    for (i = 0; jogo->revelada[i]; i++) {
        if (jogo->revelada[i] == ' ') printf("   ");
        else printf("%c ", jogo->revelada[i]);
    }
    printf("\nLetras tentadas: ");
    for (i = 0; i < 26; i++) if (jogo->tentadas[i]) { printf("%c ", (int)('A' + i)); alguma = 1; }
    if (!alguma) printf("nenhuma");
    printf("\nErros restantes: %d\n", MAX_ERROS - jogo->erros);
}

