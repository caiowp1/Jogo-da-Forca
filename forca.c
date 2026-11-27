#include <stdio.h>
#include "jogo.h"
#include "terminal.h"

int main(void) {
    char jogadores[2][10], tema[30], palavra[TAM_PALAVRA];
    int pontos[2] = {0, 0}, adivinhador, opcao;
    puts("=== JOGO DA FORCA ===\nDuas pessoas jogam: uma escolhe a frase e a outra adivinha.\nSao permitidos 4 erros por rodada. Use letras sem acento e espacos.");
    if (!terminal_ler_texto("Nome do jogador 1 (ate 9 caracteres):", jogadores[0], sizeof jogadores[0]) ||
        !terminal_ler_texto("Nome do jogador 2 (ate 9 caracteres):", jogadores[1], sizeof jogadores[1])) return 0;
    printf("Quem adivinha primeiro? 1. %s  2. %s\n", jogadores[0], jogadores[1]);
    opcao = terminal_ler_opcao("Digite 1 ou 2:");
    if (!opcao) return 0;
    adivinhador = opcao - 1;
    do {
        Jogo jogo;
        printf("\n=== PLACAR: %s %d x %d %s ===\n", jogadores[0], pontos[0], pontos[1], jogadores[1]);
        printf("%s vai adivinhar. %s, escolha o tema e a palavra; o outro jogador deve desviar o olhar.\n", jogadores[adivinhador], jogadores[1-adivinhador]);
        if (!terminal_ler_texto("Tema (ate 29 caracteres):", tema, sizeof tema)) return 0;
        do {
            if (!terminal_ler_texto("Palavra ou frase (ate 19 caracteres, letras sem acento e espacos):", palavra, sizeof palavra)) return 0;
            if (jogo_iniciar(&jogo, palavra)) break;
            puts("Use letras de A a Z e espacos, sem acentos; inclua ao menos uma letra.");
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
        printf("PLACAR: %s %d x %d %s\n", jogadores[0], pontos[0], pontos[1], jogadores[1]);
        opcao = terminal_ler_opcao("Digite 1 para encerrar ou 2 para jogar novamente:");
        if (!opcao) return 0;
        adivinhador = 1 - adivinhador;
    } while (opcao == 2);
    puts("Obrigado por jogar!");
    return 0;
}

