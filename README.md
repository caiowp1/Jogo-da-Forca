# Jogo da Forca

Jogo de terminal para duas pessoas. Uma escolhe o tema e a palavra, e a outra tenta adivinha-la antes de cometer quatro erros. A pessoa que vencer a rodada recebe um ponto.

## Compilar e executar

Instale um compilador C com suporte a C99, como GCC ou Clang. No terminal, na pasta do projeto:

```sh
gcc -std=c99 -Wall -Wextra -pedantic forca.c -o forca
./forca
```

No Windows, execute `forca.exe` (ou `.\forca.exe` no PowerShell). Se usar outro compilador, substitua `gcc` pelo comando correspondente. O compilador precisa estar disponivel no PATH.

## Como jogar

- Informe os nomes (ate 9 caracteres cada) e escolha quem vai adivinhar.
- A outra pessoa informa um tema (ate 29 caracteres) e uma palavra ou frase de ate 19 caracteres, incluindo espacos. Use letras de A a Z sem acentos e pelo menos uma letra; maiusculas e minusculas sao equivalentes. Os espacos aparecem desde o inicio.
- Enquanto a palavra e digitada, quem vai adivinhar deve desviar o olhar. O terminal pode manter o historico visivel ao rolar a tela.
- Digite uma letra por tentativa. Letras repetidas e entradas invalidas nao gastam tentativas.
- Ao final da rodada, escolha se deseja continuar. O placar fica disponivel ate o programa terminar.

