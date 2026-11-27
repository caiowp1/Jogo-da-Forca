#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "jogo.h"
#include "terminal.h"
#include "placar.h"

int main(void) {
    char jogadores[2][10], tema[30], palavra[TAM_PALAVRA];
    int pontos[2] = {0, 0}, adivinhador, opcao;
    const char *arquivo = "placar-forca.txt";
#ifdef _WIN32
    if (GetConsoleWindow()) { SetConsoleCP(CP_UTF8); SetConsoleOutputCP(CP_UTF8); }
#endif
    puts("=== JOGO DA FORCA ===\nDuas pessoas jogam: uma escolhe a frase e a outra adivinha.\nLetras e espacos sao aceitos; acentos sao revelados pela letra sem acento.");
    if (!terminal_ler_texto("Nome do jogador 1 (ate 9 caracteres):", jogadores[0], sizeof jogadores[0]) ||
        !terminal_ler_texto("Nome do jogador 2 (ate 9 caracteres):", jogadores[1], sizeof jogadores[1])) return 0;
    if (!strcmp(jogadores[0], jogadores[1])) { puts("Os nomes devem ser diferentes para identificar o placar."); return 1; }
    if (!placar_ler(arquivo, jogadores[0], &pontos[0]) || !placar_ler(arquivo, jogadores[1], &pontos[1])) {
        puts("Nao foi possivel ler o placar salvo. O jogo vai continuar com o placar zerado.");
        pontos[0] = pontos[1] = 0;
    }
    printf("Quem adivinha primeiro? 1. %s  2. %s\n", jogadores[0], jogadores[1]);
    opcao = terminal_ler_opcao("Digite 1 ou 2:");
    if (!opcao) return 0;
    adivinhador = opcao - 1;
    do {
        Jogo jogo;
        int limite;
        printf("\n=== PLACAR: %s %d x %d %s ===\n", jogadores[0], pontos[0], pontos[1], jogadores[1]);
        printf("%s vai adivinhar. %s, escolha o tema e a palavra; o outro jogador deve desviar o olhar.\n", jogadores[adivinhador], jogadores[1-adivinhador]);
        limite = terminal_ler_dificuldade();
        if (!limite) return 0;
        if (!terminal_ler_texto("Tema (ate 29 caracteres):", tema, sizeof tema)) return 0;
        do {
            if (!terminal_ler_segredo("Palavra ou frase (ate 19 letras e espacos, acentos aceitos). A digitacao fica oculta:", palavra, sizeof palavra)) return 0;
            if (jogo_iniciar(&jogo, palavra, limite)) break;
            puts("Use ate 19 letras portuguesas e espacos; inclua ao menos uma letra.");
        } while (1);
        terminal_esconder_palavra();
        while (jogo_estado(&jogo) == EM_ANDAMENTO) {
            char letra;
            ResultadoTentativa resultado;
            terminal_mostrar_jogo(&jogo, tema);
            if (!terminal_ler_letra(&letra)) return 0;
            resultado = jogo_tentar(&jogo, letra);
            if (resultado == REPETIDA) puts("Essa letra ja foi tentada; voce nao perdeu uma chance.");
            else if (resultado == ACERTOU) puts("Letra correta!");
            else if (resultado == ERROU) puts("Letra incorreta!");
        }
        terminal_mostrar_jogo(&jogo, tema);
        if (jogo_estado(&jogo) == VITORIA) { printf("\n*** %s venceu a rodada! ***\n", jogadores[adivinhador]); pontos[adivinhador]++; }
        else { printf("\n*** %s venceu! A resposta era: %s ***\n", jogadores[1-adivinhador], jogo.palavra); pontos[1-adivinhador]++; }
        if (!placar_registrar(arquivo, jogadores[jogo_estado(&jogo) == VITORIA ? adivinhador : 1-adivinhador]))
            puts("Aviso: nao foi possivel salvar o resultado no placar.");
        printf("PLACAR: %s %d x %d %s\n", jogadores[0], pontos[0], pontos[1], jogadores[1]);
        opcao = terminal_ler_opcao("Digite 1 para encerrar ou 2 para jogar novamente:");
        if (!opcao) return 0;
        adivinhador = 1 - adivinhador;
    } while (opcao == 2);
    puts("Obrigado por jogar!");
    return 0;
}

