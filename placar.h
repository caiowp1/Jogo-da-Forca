#ifndef PLACAR_H
#define PLACAR_H
int placar_ler(const char *arquivo, const char *nome, int *pontos);
int placar_registrar(const char *arquivo, const char *vencedor);
int placar_mostrar_ranking(const char *arquivo);
int placar_mostrar_historico(const char *arquivo);
#endif

