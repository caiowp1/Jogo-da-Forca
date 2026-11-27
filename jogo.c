#include "jogo.h"
#include <string.h>

static char minuscula(char c) {
    return c >= 'A' && c <= 'Z' ? (char)(c + ('a' - 'A')) : c;
}

int jogo_iniciar(Jogo *jogo, const char *palavra) {
    size_t i, tamanho = strlen(palavra);
    int tem_letra = 0;
    if (tamanho == 0 || tamanho >= TAM_PALAVRA) return 0;
    for (i = 0; i < tamanho; i++) {
        char c = minuscula(palavra[i]);
        if (c >= 'a' && c <= 'z') tem_letra = 1;
        else if (c != ' ') return 0;
    }
    if (!tem_letra) return 0;
    memset(jogo, 0, sizeof *jogo);
    for (i = 0; i < tamanho; i++) {
        jogo->palavra[i] = minuscula(palavra[i]);
        jogo->revelada[i] = palavra[i] == ' ' ? ' ' : '_';
    }
    return 1;
}

EstadoJogo jogo_estado(const Jogo *jogo) {
    if (strcmp(jogo->palavra, jogo->revelada) == 0) return VITORIA;
    return jogo->erros >= MAX_ERROS ? DERROTA : EM_ANDAMENTO;
}

ResultadoTentativa jogo_tentar(Jogo *jogo, char letra) {
    size_t i;
    int encontrou = 0;
    letra = minuscula(letra);
    if (jogo_estado(jogo) != EM_ANDAMENTO) return ENCERRADA;
    if (letra < 'a' || letra > 'z') return INVALIDA;
    if (jogo->tentadas[letra - 'a']) return REPETIDA;
    jogo->tentadas[letra - 'a'] = 1;
    for (i = 0; jogo->palavra[i] != '\0'; i++) {
        if (jogo->palavra[i] == letra) {
            jogo->revelada[i] = letra;
            encontrou = 1;
        }
    }
    if (encontrou) return ACERTOU;
    jogo->erros++;
    return ERROU;
}

