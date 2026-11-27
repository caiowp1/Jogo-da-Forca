#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif
#include "terminal.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#endif

#ifdef _WIN32
static HANDLE console_secreto;
static DWORD modo_original;
static BOOL WINAPI restaurar_em_interrupcao(DWORD evento) {
    if (evento == CTRL_C_EVENT || evento == CTRL_BREAK_EVENT || evento == CTRL_CLOSE_EVENT) {
        SetConsoleMode(console_secreto, modo_original);
    }
    return FALSE;
}
#else
static volatile sig_atomic_t sinal_recebido;
static void guardar_sinal(int sinal) { sinal_recebido = sinal; }
#endif

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

int terminal_ler_segredo(const char *pedido, char *destino, size_t tamanho) {
    int resultado;
#ifdef _WIN32
    HANDLE entrada = GetStdHandle(STD_INPUT_HANDLE);
    DWORD original;
    if (!_isatty(_fileno(stdin))) return terminal_ler_texto(pedido, destino, tamanho);
    if (entrada == INVALID_HANDLE_VALUE || !GetConsoleMode(entrada, &original)) {
        fputs("Nao foi possivel ocultar a entrada. Jogo encerrado.\n", stderr);
        return 0;
    }
    console_secreto = entrada;
    modo_original = original;
    if (!SetConsoleCtrlHandler(restaurar_em_interrupcao, TRUE)) {
        fputs("Nao foi possivel proteger a entrada. Jogo encerrado.\n", stderr);
        return 0;
    }
    if (!SetConsoleMode(entrada, original & ~(ENABLE_ECHO_INPUT))) {
        SetConsoleCtrlHandler(restaurar_em_interrupcao, FALSE);
        fputs("Nao foi possivel ocultar a entrada. Jogo encerrado.\n", stderr);
        return 0;
    }
    resultado = terminal_ler_texto(pedido, destino, tamanho);
    if (!SetConsoleMode(entrada, original)) {
        SetConsoleCtrlHandler(restaurar_em_interrupcao, FALSE);
        fputs("Nao foi possivel restaurar a configuracao do console.\n", stderr);
        return 0;
    }
    SetConsoleCtrlHandler(restaurar_em_interrupcao, FALSE);
#else
    struct termios original, oculto;
    struct sigaction acao, anteriores[3];
    const int sinais[] = {SIGINT, SIGTERM, SIGHUP};
    size_t i, instalados = 0;
    int sinal;
    if (!isatty(STDIN_FILENO)) return terminal_ler_texto(pedido, destino, tamanho);
    if (tcgetattr(STDIN_FILENO, &original) != 0) {
        fputs("Nao foi possivel ocultar a entrada. Jogo encerrado.\n", stderr);
        return 0;
    }
    memset(&acao, 0, sizeof acao);
    acao.sa_handler = guardar_sinal;
    sigemptyset(&acao.sa_mask);
    sinal_recebido = 0;
    for (i = 0; i < 3; i++) {
        if (sigaction(sinais[i], &acao, &anteriores[i]) != 0) break;
        instalados++;
    }
    if (instalados != 3) {
        while (instalados) { instalados--; sigaction(sinais[instalados], &anteriores[instalados], NULL); }
        fputs("Nao foi possivel proteger a entrada. Jogo encerrado.\n", stderr);
        return 0;
    }
    oculto = original;
    oculto.c_lflag &= (tcflag_t)~(ECHO | ECHONL);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oculto) != 0) {
        for (i = 0; i < 3; i++) sigaction(sinais[i], &anteriores[i], NULL);
        fputs("Nao foi possivel ocultar a entrada. Jogo encerrado.\n", stderr);
        return 0;
    }
    resultado = terminal_ler_texto(pedido, destino, tamanho);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &original) != 0) {
        fputs("Nao foi possivel restaurar a configuracao do terminal.\n", stderr);
        resultado = 0;
    }
    sinal = sinal_recebido;
    for (i = 0; i < 3; i++) sigaction(sinais[i], &anteriores[i], NULL);
    if (sinal) raise(sinal);
#endif
    putchar('\n');
    return resultado;
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

int terminal_ler_dificuldade(void) {
    char entrada[32];
    for (;;) {
        puts("Dificuldade: 1. Facil (6 erros)  2. Normal (4 erros)  3. Dificil (3 erros)");
        if (!ler_linha(entrada, sizeof entrada)) return 0;
        if (!strcmp(entrada, "1")) return 6;
        if (!strcmp(entrada, "2")) return 4;
        if (!strcmp(entrada, "3")) return 3;
        puts("Opcao invalida. Digite 1, 2 ou 3.");
    }
}

int terminal_ler_menu(void) {
    char entrada[32];
    for (;;) {
        puts("\n1. Jogar  2. Ranking  3. Historico de vitorias  4. Sair");
        if (!ler_linha(entrada, sizeof entrada)) return 0;
        if (strlen(entrada) == 1 && entrada[0] >= '1' && entrada[0] <= '4') return entrada[0] - '0';
        puts("Opcao invalida. Digite 1, 2, 3 ou 4.");
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
        " ___\n/  |\n|  O\n|  |\n|\n|_____", " ___\n/  |\n|  O\n| /|\n|\n|_____",
        " ___\n/  |\n|  O\n| /|\\\n|\n|_____",
        " ___\n/  |\n|  O\n| /|\\\n| /\n|_____",
        " ___\n/  |\n|  O\n| /|\\\n| / \\\n|_____"
    };
    size_t i;
    int alguma = 0;
    printf("\n--- TEMA: %s ---\n", tema);
    puts(desenhos[jogo->erros * LIMITE_ERROS / jogo->max_erros]);
    printf("Palavra: ");
    for (i = 0; jogo->revelada[i]; i++) {
        unsigned char c = (unsigned char)jogo->revelada[i];
        if (c == ' ') printf("   ");
        else if (c >= 0xc0 && jogo->revelada[i + 1]) { printf("%c%c ", c, jogo->revelada[++i]); }
        else printf("%c ", c);
    }
    printf("\nLetras tentadas: ");
    for (i = 0; i < 26; i++) if (jogo->tentadas[i]) { printf("%c ", (int)('A' + i)); alguma = 1; }
    if (!alguma) printf("nenhuma");
    printf("\nErros restantes: %d\n", jogo->max_erros - jogo->erros);
}

