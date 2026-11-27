# Jogo da Forca

Jogo de terminal para duas pessoas. Uma escolhe o tema e a palavra; a outra tem quatro erros para adivinhar. Quem vence ganha um ponto. Os papeis se alternam a cada rodada.

## Compilar e executar

Com GCC ou Clang e suporte a C99, na pasta do projeto:

```sh
gcc -std=c99 -Wall -Wextra -pedantic forca.c jogo.c terminal.c -o forca
./forca
```

No PowerShell, execute `.\forca.exe`. O compilador precisa estar no PATH. Para compilar pelo VS Code no Windows, use a tarefa de compilacao incluida.

## Como jogar

- Informe os nomes (ate 9 caracteres), escolha quem adivinha primeiro e informe um tema (ate 29 caracteres).
- A pessoa desafiante informa uma palavra ou frase de ate 19 caracteres, incluindo espacos. Use letras de A a Z sem acentos; maiusculas e minusculas sao equivalentes. Os espacos aparecem desde o inicio.
- Quem adivinha deve desviar o olhar durante a digitacao. O terminal pode manter a palavra no historico ao rolar a tela.
- Letras ja tentadas aparecem na tela. Repeticoes e entradas invalidas nao gastam tentativas. O placar permanece ate o programa terminar.

## Testar as regras

```sh
gcc -std=c99 -Wall -Wextra -pedantic jogo.c teste_jogo.c -o teste_jogo
./teste_jogo
```

No Windows, execute `.\teste_jogo.exe`. O teste termina sem mensagem quando passa.

`jogo.c` contem as regras; `terminal.c`, a entrada e exibicao; e `forca.c`, o fluxo das rodadas.

