#include "placar.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int placar_ler(const char *arquivo, const char *nome, int *pontos) {
    char linha[256];
    FILE *f = fopen(arquivo, "r");
    *pontos = 0;
    if (!f) return errno == ENOENT;
    while (fgets(linha, sizeof linha, f)) {
        size_t n = strlen(linha);
        if (n && linha[n - 1] == '\n') linha[--n] = '\0';
        if (!strcmp(linha, nome)) (*pontos)++;
    }
    if (ferror(f)) { fclose(f); return 0; }
    return fclose(f) == 0;
}

int placar_registrar(const char *arquivo, const char *vencedor) {
    FILE *f = fopen(arquivo, "a");
    int ok;
    if (!f) return 0;
    ok = fprintf(f, "%s\n", vencedor) >= 0;
    return fclose(f) == 0 && ok;
}

