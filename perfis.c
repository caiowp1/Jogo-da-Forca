#include "perfis.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

static int valido(const char *nome) {
    size_t i, n = strlen(nome);
    if (!n || n >= PERFIL_NOME || nome[0] == ' ' || nome[n-1] == ' ') return 0;
    for (i=0;i<n;i++) if ((unsigned char)nome[i] < 32 || (unsigned char)nome[i] == 127) return 0;
    return 1;
}
static int igual(const char *a, const char *b) {
    while (*a && *b) {
        unsigned char x=(unsigned char)*a++, y=(unsigned char)*b++;
        if(x>='A' && x<='Z') x=(unsigned char)(x+32);
        if(y>='A' && y<='Z') y=(unsigned char)(y+32);
        if(x!=y) return 0;
    }
    return *a == *b;
}
int perfis_carregar(Perfis *p, const char *caminho) {
    FILE *f=fopen(caminho,"r"); char linha[256]; Perfis novo={0};
    if(!f) { if(errno==ENOENT) { *p=novo; return 1; } return 0; }
    while(fgets(linha,sizeof linha,f)) {
        size_t n=strlen(linha); int i;
        if(!n || linha[n-1]!='\n') { fclose(f); return 0; }
        linha[--n]=0;
        if(!valido(linha) || novo.total==PERFIS_MAX) { fclose(f); return 0; }
        for(i=0;i<novo.total;i++) if(igual(novo.nomes[i],linha)) { fclose(f); return 0; }
        strcpy(novo.nomes[novo.total++],linha);
    }
    if(ferror(f)) { fclose(f); return 0; }
    if(fclose(f)!=0) return 0;
    *p=novo; return 1;
}
PerfilResultado perfis_adicionar(Perfis *p, const char *caminho, const char *nome) {
    int i, ok; FILE *f; Perfis atual;
    if(!valido(nome)) return PERFIL_INVALIDO;
    if(!perfis_carregar(&atual,caminho)) return PERFIL_ERRO;
    for(i=0;i<atual.total;i++) if(igual(atual.nomes[i],nome)) return PERFIL_DUPLICADO;
    if(atual.total==PERFIS_MAX) return PERFIL_CHEIO;
    f=fopen(caminho,"a"); if(!f) return PERFIL_ERRO;
    ok=fprintf(f,"%s\n",nome)>=0;
    if(fclose(f)!=0 || !ok) return PERFIL_ERRO;
    strcpy(atual.nomes[atual.total++],nome); *p=atual;
    return PERFIL_OK;
}

