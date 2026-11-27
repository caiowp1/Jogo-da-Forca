#include "perfis.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
    Perfis p={0}, recarregado={0};
    assert(argc==2);
    assert(perfis_carregar(&p,argv[1]) && p.total==0);
    assert(perfis_adicionar(&p,argv[1],"")==PERFIL_INVALIDO);
    assert(perfis_adicionar(&p,argv[1],"  ")==PERFIL_INVALIDO);
    assert(perfis_adicionar(&p,argv[1],"muitolongo") == PERFIL_INVALIDO);
    assert(perfis_adicionar(&p,argv[1],"caio")==PERFIL_OK);
    assert(perfis_adicionar(&p,argv[1],"CAIO")==PERFIL_DUPLICADO);
    assert(perfis_adicionar(&p,argv[1],"ste")==PERFIL_OK);
    assert(perfis_carregar(&recarregado,argv[1]));
    assert(recarregado.total==2 && !strcmp(recarregado.nomes[0],"caio") && !strcmp(recarregado.nomes[1],"ste"));
    {
        FILE *f=fopen(argv[1],"a"); assert(f);
        assert(fputs("incompleto",f)>=0); assert(fclose(f)==0);
        assert(!perfis_carregar(&recarregado,argv[1]));
        assert(recarregado.total==2);
        assert(perfis_adicionar(&p,argv[1],"bia")==PERFIL_ERRO);
    }
    return 0;
}

