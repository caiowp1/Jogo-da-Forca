#ifndef JOGO_H
#define JOGO_H

#define TAM_PALAVRA 20
#define MAX_ERROS 4

typedef struct {
    char palavra[TAM_PALAVRA];
    char revelada[TAM_PALAVRA];
    int tentadas[26];
    int erros;
} Jogo;

typedef enum { INVALIDA, REPETIDA, ACERTOU, ERROU, ENCERRADA } ResultadoTentativa;
typedef enum { EM_ANDAMENTO, VITORIA, DERROTA } EstadoJogo;

/* Aceita letras ASCII e espacos, com pelo menos uma letra. */
int jogo_iniciar(Jogo *jogo, const char *palavra);
ResultadoTentativa jogo_tentar(Jogo *jogo, char letra);
EstadoJogo jogo_estado(const Jogo *jogo);

#endif

