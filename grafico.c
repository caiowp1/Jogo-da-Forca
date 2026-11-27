#include "raylib.h"
#include "jogo.h"
#include "placar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { MENU, PREPARAR, JOGAR, RESULTADO, RANKING } Tela;
typedef struct { char nome[256]; int pontos; } Jogador;
static const Color fundo = {20, 23, 38, 255}, painel = {35, 39, 57, 255};
static const Color roxo = {166, 148, 255, 255}, verde = {207, 247, 112, 255};
static const Color texto = {240, 241, 248, 255}, suave = {174, 180, 199, 255};
static const char *arquivo = "placar-forca.txt";

static int botao(Rectangle r, const char *s, Color cor, Color tinta) {
    int hover = CheckCollisionPointRec(GetMousePosition(), r);
    DrawRectangleRounded(r, .23f, 8, hover ? ColorBrightness(cor, .12f) : cor);
    DrawText(s, (int)(r.x + (r.width - MeasureText(s, 19))/2), (int)(r.y + (r.height - 19)/2), 19, tinta);
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void campo(Rectangle r, const char *titulo, const char *valor, int ativo, int secreto) {
    DrawText(titulo, (int)r.x, (int)r.y - 24, 17, suave);
    DrawRectangleRounded(r, .18f, 6, (Color){27,30,44,255});
    DrawRectangleRoundedLinesEx(r, .18f, 6, 2, ativo ? verde : (Color){75,79,98,255});
    if (secreto) valor = "Digitacao oculta";
    BeginScissorMode((int)r.x + 9, (int)r.y, (int)r.width - 18, (int)r.height);
    DrawText(valor, (int)r.x + 12, (int)r.y + 14, 20, texto);
    EndScissorMode();
}

static void inserir(char *s, size_t cap, int cp, int max_caracteres) {
    int bytes, count = 0; size_t n = strlen(s), i;
    const char *utf8;
    if (cp < 32 || cp == 127) return;
    for (i = 0; i < n; i++) if (((unsigned char)s[i] & 0xc0) != 0x80) count++;
    utf8 = CodepointToUTF8(cp, &bytes);
    if (count < max_caracteres && n + (size_t)bytes < cap) { memcpy(s+n, utf8, (size_t)bytes); s[n+bytes] = 0; }
}
static void apagar(char *s) {
    size_t n = strlen(s); if (!n) return;
    do { n--; } while (n && ((unsigned char)s[n] & 0xc0) == 0x80);
    s[n] = 0;
}

static int comparar(const void *a, const void *b) {
    const Jogador *x=a, *y=b;
    if (x->pontos != y->pontos) return x->pontos < y->pontos ? 1 : -1;
    return strcmp(x->nome, y->nome);
}
static void ranking(void) {
    FILE *f = fopen(arquivo, "r"); Jogador *lista=NULL; size_t total=0, i; char linha[256];
    DrawText("Ranking de vitorias - 10 primeiros", 58, 90, 30, texto);
    if (!f) { DrawText("Nenhum placar disponivel ainda.", 60, 155, 20, suave); return; }
    while (fgets(linha, sizeof linha, f)) {
        size_t n=strlen(linha); Jogador *tmp;
        if (!n || linha[n-1]!='\n') continue;
        linha[--n]=0; if (!n) continue;
        for (i=0; i<total && strcmp(lista[i].nome,linha); i++) {}
        if(i==total) { tmp=realloc(lista,(total+1)*sizeof *lista); if(!tmp) break; lista=tmp; strcpy(lista[i].nome,linha);lista[i].pontos=0;total++; }
        lista[i].pontos++;
    }
    fclose(f);
    if (!total) DrawText("Nenhuma vitoria registrada ainda.", 60, 155, 20, suave);
    else { qsort(lista,total,sizeof *lista,comparar); for(i=0;i<total && i<10;i++) DrawText(TextFormat("%i.  %s   -   %i vitoria(s)",(int)i+1,lista[i].nome,lista[i].pontos), 65, 150+(int)i*32, 21, texto); }
    free(lista);
}

static void forca(int erros, int max) {
    int etapa=erros*6/max;
    DrawLineEx((Vector2){95,390},(Vector2){310,390},7,roxo);
    DrawLineEx((Vector2){140,390},(Vector2){140,135},7,roxo);
    DrawLineEx((Vector2){140,135},(Vector2){255,135},7,roxo);
    DrawLineEx((Vector2){255,135},(Vector2){255,162},7,roxo);
    if(etapa>=1) DrawCircleLines(255,185,23,verde);
    if(etapa>=2) DrawLineEx((Vector2){255,208},(Vector2){255,282},5,verde);
    if(etapa>=3) DrawLineEx((Vector2){255,225},(Vector2){220,252},5,verde);
    if(etapa>=4) DrawLineEx((Vector2){255,225},(Vector2){290,252},5,verde);
    if(etapa>=5) DrawLineEx((Vector2){255,282},(Vector2){225,325},5,verde);
    if(etapa>=6) DrawLineEx((Vector2){255,282},(Vector2){285,325},5,verde);
}

int main(void) {
    Tela tela=MENU; Jogo jogo; int campo_ativo=0, adiv=0, limite=4, pontos[2]={0,0}, salvo=1;
    char nomes[2][10]={{0}}, tema[30]={0}, segredo[TAM_PALAVRA]={0}, mensagem[160]={0};
    char *campos[]={nomes[0],nomes[1],tema,segredo};
    const size_t caps[]={sizeof nomes[0],sizeof nomes[1],sizeof tema,sizeof segredo};
    const int limites[]={9,9,29,60};
    int i;
    SetConfigFlags(FLAG_VSYNC_HINT); InitWindow(960, 680, "Jogo da Forca"); SetTargetFPS(60);
    while(!WindowShouldClose()) {
        int click=IsMouseButtonPressed(MOUSE_BUTTON_LEFT), cp;
        BeginDrawing(); ClearBackground(fundo);
        DrawText("JOGO DA FORCA", 40, 24, 24, verde);
        DrawText("DUAS PESSOAS  /  UMA PALAVRA", 650, 30, 14, suave);
        if(tela==MENU) {
            DrawText("Uma letra de", 70, 135, 48, texto); DrawText("cada vez.", 70, 190, 48, verde);
            DrawText("Chame alguem e comece uma rodada.", 72, 267, 20, suave);
            if(botao((Rectangle){70,330,240,52},"Jogar",verde,fundo)) tela=PREPARAR;
            if(botao((Rectangle){70,397,240,52},"Ver ranking",painel,texto)) tela=RANKING;
            if(botao((Rectangle){70,464,240,52},"Sair",painel,texto)) { EndDrawing(); CloseWindow(); return 0; }
            BeginMode2D((Camera2D){.offset={460,25},.zoom=1}); forca(4,6); EndMode2D();
        } else if(tela==PREPARAR) {
            Rectangle caixas[]={{80,147,360,48},{500,147,360,48},{80,239,780,48},{80,331,780,48}};
            DrawText("Prepare a rodada", 60, 74, 34, texto);
            DrawText("Quem vai adivinhar deve desviar o olhar.", 80, 625, 17, suave);
            for(i=0;i<4;i++) { if(click && CheckCollisionPointRec(GetMousePosition(),caixas[i])) campo_ativo=i; }
            campo(caixas[0],"Jogador 1",nomes[0],campo_ativo==0,0); campo(caixas[1],"Jogador 2",nomes[1],campo_ativo==1,0);
            campo(caixas[2],"Tema",tema,campo_ativo==2,0); campo(caixas[3],"Palavra ou frase secreta (ate 60 caracteres)",segredo,campo_ativo==3,1);
            while((cp=GetCharPressed())>0) inserir(campos[campo_ativo],caps[campo_ativo],cp,limites[campo_ativo]);
            if(IsKeyPressed(KEY_BACKSPACE)) apagar(campos[campo_ativo]);
            if(IsKeyPressed(KEY_TAB)) campo_ativo=(campo_ativo+1)%4;
            DrawText("Quem adivinha?",80,415,18,suave);
            if(botao((Rectangle){80,445,150,40},"Jogador 1",adiv==0?roxo:painel,texto)) adiv=0;
            if(botao((Rectangle){240,445,150,40},"Jogador 2",adiv==1?roxo:painel,texto)) adiv=1;
            DrawText("Dificuldade",500,415,18,suave);
            if(botao((Rectangle){500,445,110,40},"Facil",limite==6?roxo:painel,texto)) limite=6;
            if(botao((Rectangle){620,445,110,40},"Normal",limite==4?roxo:painel,texto)) limite=4;
            if(botao((Rectangle){740,445,120,40},"Dificil",limite==3?roxo:painel,texto)) limite=3;
            DrawText(mensagem,80,505,18,(Color){255,160,160,255});
            if(botao((Rectangle){80,550,190,48},"Voltar",painel,texto)) {tela=MENU;mensagem[0]=0;}
            if(botao((Rectangle){600,550,260,48},"Comecar rodada",verde,fundo)) {
                if(!nomes[0][0] || !nomes[1][0] || !tema[0] || !strcmp(nomes[0],nomes[1])) snprintf(mensagem,sizeof mensagem,"Informe nomes diferentes e um tema.");
                else if(!jogo_iniciar(&jogo,segredo,limite)) snprintf(mensagem,sizeof mensagem,"Use letras portuguesas e espacos na palavra.");
                else { if(!placar_ler(arquivo,nomes[0],&pontos[0]) || !placar_ler(arquivo,nomes[1],&pontos[1])) snprintf(mensagem,sizeof mensagem,"Nao foi possivel ler o placar."); else mensagem[0]=0; memset(segredo,0,sizeof segredo); tela=JOGAR; }
            }
        } else if(tela==JOGAR || tela==RESULTADO) {
            int letra=0; char exibida[256]; size_t p=0,k=0, pos=0;
            const char *mostrar = tela==RESULTADO ? jogo.palavra : jogo.revelada;
            DrawText(TextFormat("%s adivinha",nomes[adiv]),390,85,26,texto);
            DrawText(TextFormat("Tema: %s",tema),390,126,19,suave);
            DrawText(TextFormat("%s %d  x  %d %s",nomes[0],pontos[0],pontos[1],nomes[1]),390,155,19,roxo);
            forca(jogo.erros,jogo.max_erros);
            while(mostrar[p] && k+4<sizeof exibida) { unsigned char c=(unsigned char)mostrar[p++]; if(pos && pos%22==0) exibida[k++]='\n'; exibida[k++]=(char)c; if(c>=0xc0 && mostrar[p]) exibida[k++]=mostrar[p++]; exibida[k++]=' '; pos++; }
            exibida[k]=0;
            DrawText("PALAVRA",390,205,16,suave);
            BeginScissorMode(385,230,550,105); DrawTextEx(GetFontDefault(),exibida,(Vector2){390,238},20,1,texto); EndScissorMode();
            DrawText(TextFormat("Erros restantes: %d",jogo.max_erros-jogo.erros),390,305,19,verde);
            if(tela==JOGAR) {
                for(i=0;i<26;i++) { Rectangle r={(float)(390+(i%9)*56),(float)(355+(i/9)*52),48,43}; char s[2]={(char)('A'+i),0}; if(botao(r,s,jogo.tentadas[i]?painel:roxo,jogo.tentadas[i]?suave:fundo)) letra='a'+i; }
                while((cp=GetCharPressed())>0) { if(cp>='A' && cp<='Z') letra=cp; else if(cp>='a' && cp<='z') letra=cp; }
                if(letra) { ResultadoTentativa r=jogo_tentar(&jogo,(char)letra); if(r==REPETIDA) snprintf(mensagem,sizeof mensagem,"Letra ja tentada."); else mensagem[0]=0; }
                DrawText(mensagem,390,525,18,suave);
                if(jogo_estado(&jogo)!=EM_ANDAMENTO) { int vencedor=jogo_estado(&jogo)==VITORIA?adiv:1-adiv; pontos[vencedor]++; salvo=placar_registrar(arquivo,nomes[vencedor]); snprintf(mensagem,sizeof mensagem,"%s venceu!",nomes[vencedor]); tela=RESULTADO; }
            } else {
                DrawText(mensagem,390,355,26,verde);
                if(!salvo) DrawText("Nao foi possivel salvar o placar.",390,525,17,(Color){255,160,160,255});
                if(botao((Rectangle){390,455,230,48},"Proxima rodada",verde,fundo)) { tela=PREPARAR; adiv=1-adiv; tema[0]=0; campo_ativo=2; mensagem[0]=0; }
                if(botao((Rectangle){635,455,200,48},"Menu inicial",painel,texto)) { tela=MENU; tema[0]=0; mensagem[0]=0; }
            }
        } else if(tela==RANKING) { ranking(); if(botao((Rectangle){60,570,200,48},"Voltar",painel,texto)) tela=MENU; }
        EndDrawing();
    }
    CloseWindow(); return 0;
}

