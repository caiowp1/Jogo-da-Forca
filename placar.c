#include "placar.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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

typedef struct { char nome[256]; int pontos; } Entrada;

static int comparar(const void *a, const void *b) {
    const Entrada *x = a, *y = b;
    if (x->pontos != y->pontos) return x->pontos < y->pontos ? 1 : -1;
    return strcmp(x->nome, y->nome);
}

int placar_mostrar_ranking(const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    int erro_abertura = errno;
    Entrada *lista = NULL;
    size_t total = 0, i;
    char linha[256];
    puts("\n=== RANKING DE VITORIAS ===");
    if (!f) {
        if (erro_abertura == ENOENT) { puts("Ainda nao ha vitorias registradas."); return 1; }
        return 0;
    }
    while (fgets(linha, sizeof linha, f)) {
        size_t n = strlen(linha);
        Entrada *nova;
        if (!n || linha[n - 1] != '\n') { free(lista); fclose(f); return 0; }
        linha[--n] = '\0';
        if (!n) continue;
        for (i = 0; i < total && strcmp(lista[i].nome, linha); i++) {}
        if (i == total) {
            nova = realloc(lista, (total + 1) * sizeof *lista);
            if (!nova) { free(lista); fclose(f); return 0; }
            lista = nova;
            strcpy(lista[total].nome, linha);
            lista[total].pontos = 0;
            total++;
        }
        lista[i].pontos++;
    }
    if (ferror(f)) { free(lista); fclose(f); return 0; }
    if (fclose(f) != 0) { free(lista); return 0; }
    if (total == 0) puts("Ainda nao ha vitorias registradas.");
    else {
        qsort(lista, total, sizeof *lista, comparar);
        for (i = 0; i < total; i++) printf("%zu. %s - %d vitoria(s)\n", i + 1, lista[i].nome, lista[i].pontos);
    }
    free(lista);
    return 1;
}

int placar_mostrar_historico(const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    int erro_abertura = errno;
    char linha[256];
    unsigned long numero = 0;
    puts("\n=== HISTORICO DE VITORIAS ===");
    if (!f) {
        if (erro_abertura == ENOENT) { puts("Ainda nao ha vitorias registradas."); return 1; }
        return 0;
    }
    while (fgets(linha, sizeof linha, f)) {
        size_t n = strlen(linha);
        if (!n || linha[n - 1] != '\n') { fclose(f); return 0; }
        linha[--n] = '\0';
        if (n) printf("%lu. %s\n", ++numero, linha);
    }
    if (ferror(f)) { fclose(f); return 0; }
    if (fclose(f) != 0) return 0;
    if (!numero) puts("Ainda nao ha vitorias registradas.");
    return 1;
}

