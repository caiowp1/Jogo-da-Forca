# Jogo da Forca

Jogo de terminal para duas pessoas. Uma escolhe o tema e a palavra; a outra tenta adivinhar. Quem vence ganha um ponto. Os papeis se alternam a cada rodada.

## Compilar e executar

Com GCC ou Clang e suporte a C99, na pasta do projeto:

```sh
gcc -std=c99 -Wall -Wextra -pedantic forca.c jogo.c terminal.c placar.c -o forca
./forca
```

No PowerShell, execute `.\forca.exe`. O compilador precisa estar no PATH. Para compilar pelo VS Code no Windows, use a tarefa de compilacao incluida.

## Como jogar

- Informe os nomes (ate 9 caracteres), escolha quem adivinha primeiro e selecione a dificuldade em cada rodada: facil (6 erros), normal (4 erros) ou dificil (3 erros). Depois informe um tema (ate 29 caracteres).
- A pessoa desafiante informa uma palavra ou frase de ate 60 caracteres, incluindo espacos. Acentos portugueses e cedilha sao aceitos; maiusculas e minusculas sao equivalentes. Para adivinhar, digite a letra sem acento: `a` revela tambem `á`, `â` e `ã`, por exemplo. Os espacos aparecem desde o inicio. Em Linux e macOS use um terminal UTF-8; no Windows o programa configura o console para UTF-8.
- Ao digitar a palavra em um terminal, a entrada fica oculta, sem asteriscos. Em entradas redirecionadas, como testes por arquivo, a leitura funciona normalmente. Ao fim da rodada, a palavra e exibida no resultado.
- Letras ja tentadas aparecem na tela. Repeticoes e entradas invalidas nao gastam tentativas. Cada vitoria e salva em `placar-forca.txt` na pasta em que o programa foi iniciado e carregada pelo nome na proxima partida. Use nomes diferentes e exatamente iguais aos das partidas anteriores. A pasta precisa permitir gravacao; se nao permitir, o jogo avisa.
- No menu inicial, consulte o ranking por numero de vitorias ou o historico de vencedores em ordem de registro. Os resultados antigos continuam compativeis; o historico existente nao guarda datas nem adversarios.

## Testar as regras

```sh
gcc -std=c99 -Wall -Wextra -pedantic jogo.c teste_jogo.c -o teste_jogo
./teste_jogo
```

No Windows, execute `.\teste_jogo.exe`. O teste termina sem mensagem quando passa.

`jogo.c` contem as regras; `terminal.c`, a entrada e exibicao; `placar.c`, o historico de vitorias; e `forca.c`, o fluxo das rodadas.

## Interface grafica (Windows)

A versao grafica usa [raylib 6.2](https://github.com/raysan5/raylib/releases/tag/6.2), com o pacote `win64_mingw-w64` e um compilador MinGW-w64 de 64 bits. Extraia o pacote e ajuste o caminho em `$ray` para a pasta extraida. No PowerShell, na pasta do projeto:

```powershell
$ray = 'C:\caminho\para\raylib-6.2_win64_mingw-w64'
gcc -std=c99 -Wall -Wextra -pedantic -I "$ray\include" grafico.c jogo.c placar.c perfis.c -L "$ray\lib" -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows -o forca-grafica.exe
.\forca-grafica.exe
```

No menu Perfis, cadastre cada jogador uma vez. Na preparacao, selecione dois perfis diferentes; tambem e possivel cadastrar um perfil a partir da tela de selecao. Os nomes aceitam ate 9 bytes em UTF-8 (letras acentuadas ocupam mais de um byte), nao podem comecar ou terminar com espaco e nao podem ser duplicados, mesmo com diferenca entre letras maiusculas e minusculas ASCII. Os perfis ficam em `perfis-forca.txt` na pasta de execucao; a pasta precisa permitir gravacao. Eles nao exigem senha e nao impedem outra pessoa de seleciona-los.

Se ja tiver um placar, cadastre o nome exatamente como foi usado antes (inclusive maiusculas e acentos) para manter as vitorias correspondentes. Os arquivos antigos nao sao apagados nem importados automaticamente. Ao jogar, selecione os dois perfis, clique no jogador que vai adivinhar e escolha a dificuldade. Na tela seguinte, o outro jogador informa o tema e a palavra. Clique nos campos e use Tab para alternar entre tema e palavra. A palavra secreta fica oculta; na rodada use o teclado ou clique nas letras. O ranking mostra os dez jogadores com mais vitorias e usa o mesmo arquivo `placar-forca.txt` da versao de terminal, na pasta de execucao. A versao de terminal continua disponivel com o comando anterior e continua usando nomes digitados.

