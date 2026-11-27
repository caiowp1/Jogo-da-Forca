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

