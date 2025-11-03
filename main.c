#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "score.h"

#define DEFAULT_ROWS 5
#define DEFAULT_COLS 5
#define DEFAULT_BOMBS 3
#define RANKING_FILE "ranking.txt"

int main(void) {
    char player[MAX_NAME];
    printf("Nome do jogador: ");
    if (!fgets(player, sizeof(player), stdin)) return 0;
    // remove newline
    player[strcspn(player, "\r\n")] = '\0';
    if (strlen(player) == 0) strncpy(player, "Anonimo", sizeof(player)-1);

    int rows = DEFAULT_ROWS, cols = DEFAULT_COLS, bombs = DEFAULT_BOMBS;
    printf("Usar valores padrao %dx%d com %d bombas? (s/n): ", rows, cols, bombs);
    char line[128];
    if (fgets(line, sizeof(line), stdin)) {
        if (line[0] == 'n' || line[0] == 'N') {
            printf("Linhas: "); if (fgets(line, sizeof(line), stdin)) rows = atoi(line);
            printf("Colunas: "); if (fgets(line, sizeof(line), stdin)) cols = atoi(line);
            printf("Bombas: "); if (fgets(line, sizeof(line), stdin)) bombs = atoi(line);
            if (rows <= 0) rows = DEFAULT_ROWS;
            if (cols <= 0) cols = DEFAULT_COLS;
            if (bombs <= 0 || bombs >= rows*cols) bombs = DEFAULT_BOMBS;
        }
    }

    Board *b = board_create(rows, cols, bombs);
    if (!b) { fprintf(stderr, "Erro ao alocar tabuleiro\n"); return 1; }

    int running = 1;
    int abandoned = 0;
    while (running) {
        board_print(b, 0);
        printf("Comando (r linha col = revelar, f linha col = marcar/desmarcar, l = ranking, q = desistir): ");
        if (!fgets(line, sizeof(line), stdin)) break;
        if (line[0] == 'q' || line[0] == 'Q') {
            printf("Deseja realmente desistir? (s/n): ");
            if (fgets(line, sizeof(line), stdin) && (line[0]=='s' || line[0]=='S')) {
                abandoned = 1; break;
            } else continue;
        } else if (line[0] == 'l' || line[0] == 'L') {
            print_scores(RANKING_FILE);
            continue;
        } else if (line[0] == 'f' || line[0] == 'F') {
            int r,c;
            if (sscanf(line+1, "%d %d", &r, &c) == 2) {
                if (board_in_bounds(b,r,c)) board_toggle_flag(b,r,c);
                else printf("Posicao fora dos limites\n");
            } else printf("Entrada invalida. Use: f linha col\n");
        } else if (line[0] == 'r' || line[0] == 'R') {
            int r,c;
            if (sscanf(line+1, "%d %d", &r, &c) == 2) {
                if (!board_in_bounds(b,r,c)) { printf("Posicao fora dos limites\n"); continue; }
                int res = board_reveal(b, r, c);
                if (res == 1) {
                    // detonated
                    board_reveal_all_bombs(b);
                    board_print(b, 1);
                    printf("BOOM! Voce detonou uma bomba. Fim de jogo.\n");
                    running = 0;
                } else if (res == -1) {
                    printf("Casa ja revelada ou marcada\n");
                } else {
                    // check win
                    int total_safe = b->rows * b->cols - b->bombs;
                    if (b->revealed_count == total_safe) {
                        board_print(b, 1);
                        printf("Parabens %s! Voce venceu.\n", player);
                        double esc = 100.0 * (double)b->revealed_count / (double)total_safe;
                        printf("Seu escore: %.2f\n", esc);
                        try_add_score(RANKING_FILE, player, esc);
                        running = 0;
                    }
                }
            } else printf("Entrada invalida. Use: r linha col\n");
        } else {
            printf("Comando desconhecido.\n");
        }
    }

    if (abandoned) printf("Partida abandonada. Nenhuma alteracao no placar.\n");

    board_free(b);
    printf("Obrigado por jogar.\n");
    return 0;
}
