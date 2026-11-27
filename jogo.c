#include "jogo.h"
#include <string.h>

/* Retorna a letra basica e quantos bytes UTF-8 ela ocupa. */
static char base(const unsigned char *p, size_t *bytes) {
    unsigned char c = p[0];
    *bytes = 1;
    if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
    if ((c >= 'a' && c <= 'z') || c == ' ') return (char)c;
    if (c != 0xc3 || !p[1]) return 0;
    *bytes = 2;
    switch (p[1]) {
        case 0x80: case 0x81: case 0x82: case 0x83: case 0xa0: case 0xa1: case 0xa2: case 0xa3: return 'a';
        case 0x87: case 0xa7: return 'c';
        case 0x88: case 0x89: case 0x8a: case 0xa8: case 0xa9: case 0xaa: return 'e';
        case 0x8c: case 0x8d: case 0xac: case 0xad: return 'i';
        case 0x92: case 0x93: case 0x94: case 0x95: case 0xb2: case 0xb3: case 0xb4: case 0xb5: return 'o';
        case 0x99: case 0x9a: case 0x9c: case 0xb9: case 0xba: case 0xbc: return 'u';
        default: return 0;
    }
}

static void atualizar(Jogo *j) {
    size_t p = 0, saida = 0, n, i;
    for (i = 0; j->bases[i]; i++) {
        char b = base((const unsigned char *)j->palavra + p, &n);
        if (b == ' ' || j->tentadas[b - 'a']) {
            memcpy(j->revelada + saida, j->palavra + p, n); saida += n;
        } else j->revelada[saida++] = '_';
        p += n;
    }
    j->revelada[saida] = '\0';
}

int jogo_iniciar(Jogo *j, const char *palavra, int max_erros) {
    size_t p = 0, i = 0, n;
    int tem_letra = 0;
    Jogo novo = {0};
    if (max_erros < 1 || max_erros > LIMITE_ERROS) return 0;
    novo.max_erros = max_erros;
    while (palavra[p]) {
        char b;
        if (i == MAX_LETRAS) return 0;
        b = base((const unsigned char *)palavra + p, &n);
        if (!b) return 0;
        if (b != ' ') tem_letra = 1;
        novo.bases[i++] = b;
        p += n;
    }
    if (!tem_letra) return 0;
    memcpy(novo.palavra, palavra, p + 1);
    atualizar(&novo);
    *j = novo;
    return 1;
}

EstadoJogo jogo_estado(const Jogo *j) {
    if (!strcmp(j->palavra, j->revelada)) return VITORIA;
    return j->erros >= j->max_erros ? DERROTA : EM_ANDAMENTO;
}

ResultadoTentativa jogo_tentar(Jogo *j, char letra) {
    if (jogo_estado(j) != EM_ANDAMENTO) return ENCERRADA;
    if (letra >= 'A' && letra <= 'Z') letra = (char)(letra + ('a' - 'A'));
    if (letra < 'a' || letra > 'z') return INVALIDA;
    if (j->tentadas[letra - 'a']) return REPETIDA;
    j->tentadas[letra - 'a'] = 1;
    if (strchr(j->bases, letra)) { atualizar(j); return ACERTOU; }
    j->erros++;
    return ERROU;
}

