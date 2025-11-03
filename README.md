Campo Minado (Minesweeper) - Trabalho Final

Arquivos:
- main.c: loop principal e interação com o jogador
- board.h / board.c: implementação do tabuleiro, bombas, revelação em flood-fill
- score.h / score.c: gerência do placar (arquivo ranking.txt)
- Makefile: compila a aplicação (Linux)

Como compilar (Linux):

1. Abra um terminal no diretório do projeto.
2. Rode: make

Executar:

./mines

Comandos durante o jogo:
- r linha col   -> revelar a casa (linha e coluna são inteiros, ex: r 0 0)
- f linha col   -> marcar/desmarcar com bandeira
- l             -> mostrar ranking (top 10)
- q             -> desistir (não atualiza o placar)

Observações:
- As bombas são posicionadas aleatoriamente após a primeira jogada, garantindo que a primeira casa revelada não contenha bomba.
- O placar (ranking.txt) guarda as 10 maiores pontuações; é atualizado somente quando o jogador vence.
