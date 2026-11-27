#ifndef TERMINAL_H
#define TERMINAL_H
#include <stddef.h>
#include "jogo.h"
int terminal_ler_texto(const char *pedido, char *destino, size_t tamanho);
int terminal_ler_segredo(const char *pedido, char *destino, size_t tamanho);
int terminal_ler_opcao(const char *pedido);
int terminal_ler_dificuldade(void);
int terminal_ler_menu(void);
int terminal_ler_letra(char *letra);
void terminal_esconder_palavra(void);
void terminal_mostrar_jogo(const Jogo *jogo, const char *tema);
#endif

