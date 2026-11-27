#ifndef JOGO_H
#define JOGO_H

#define MAX_LETRAS 19
#define TAM_PALAVRA (MAX_LETRAS * 4 + 1)
#define LIMITE_ERROS 6

typedef struct {
    char palavra[TAM_PALAVRA];
    char revelada[TAM_PALAVRA];
    int tentadas[26];
    int erros;
    int max_erros;
    char bases[MAX_LETRAS + 1];
} Jogo;

typedef enum { INVALIDA, REPETIDA, ACERTOU, ERROU, ENCERRADA } ResultadoTentativa;
typedef enum { EM_ANDAMENTO, VITORIA, DERROTA } EstadoJogo;

/* Aceita letras portuguesas em UTF-8 e espacos, com pelo menos uma letra. */
int jogo_iniciar(Jogo *jogo, const char *palavra, int max_erros);
ResultadoTentativa jogo_tentar(Jogo *jogo, char letra);
EstadoJogo jogo_estado(const Jogo *jogo);

#endif

