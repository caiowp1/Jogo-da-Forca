#ifndef PERFIS_H
#define PERFIS_H
#define PERFIL_NOME 10
#define PERFIS_MAX 100
typedef struct { char nomes[PERFIS_MAX][PERFIL_NOME]; int total; } Perfis;
typedef enum { PERFIL_OK, PERFIL_INVALIDO, PERFIL_DUPLICADO, PERFIL_CHEIO, PERFIL_ERRO } PerfilResultado;
int perfis_carregar(Perfis *p, const char *caminho);
PerfilResultado perfis_adicionar(Perfis *p, const char *caminho, const char *nome);
#endif

