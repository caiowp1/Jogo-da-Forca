#include "jogo.h"
#include <assert.h>
#include <string.h>

int main(void) {
    Jogo j;
    assert(!jogo_iniciar(&j, "   "));
    assert(!jogo_iniciar(&j, "ola!"));
    assert(!jogo_iniciar(&j, "abcdefghijklmnopqrst"));
    assert(jogo_iniciar(&j, "Casa Azul"));
    assert(!strcmp(j.revelada, "____ ____"));
    assert(jogo_tentar(&j, 'A') == ACERTOU);
    assert(!strcmp(j.revelada, "_a_a a___"));
    assert(jogo_tentar(&j, 'a') == REPETIDA && j.erros == 0);
    assert(jogo_tentar(&j, '1') == INVALIDA && j.erros == 0);
    assert(jogo_tentar(&j, 'c') == ACERTOU);
    assert(jogo_tentar(&j, 's') == ACERTOU);
    assert(jogo_tentar(&j, 'z') == ACERTOU);
    assert(jogo_tentar(&j, 'u') == ACERTOU);
    assert(jogo_tentar(&j, 'l') == ACERTOU);
    assert(jogo_estado(&j) == VITORIA);
    assert(jogo_tentar(&j, 'x') == ENCERRADA);
    assert(jogo_iniciar(&j, "casa"));
    assert(jogo_tentar(&j, 'x') == ERROU);
    assert(jogo_tentar(&j, 'y') == ERROU);
    assert(jogo_tentar(&j, 'z') == ERROU);
    assert(jogo_tentar(&j, 'w') == ERROU);
    assert(jogo_estado(&j) == DERROTA && j.erros == MAX_ERROS);
    return 0;
}

