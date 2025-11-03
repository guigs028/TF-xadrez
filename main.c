/**
 * CAMPO MINADO - Trabalho Final
 * Jogo Campo Minado implementado em C
 * O jogador deve revelar todas as casas sem bombas para vencer
 * Escore = 100.0 * casas_reveladas / (total_casas - bombas)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "score.h"

#define DEFAULT_ROWS 5
#define DEFAULT_COLS 5
#define DEFAULT_BOMBS 3
#define RANKING_FILE "ranking.txt"

/**
 * Calcula o escore atual do jogador
 * @param revealed Numero de casas reveladas
 * @param total_safe Total de casas sem bombas
 * @return Escore calculado
 */
double calculate_score(int revealed, int total_safe) {
    return 100.0 * (double)revealed / (double)total_safe;
}

int main(void) {
    char player[MAX_NAME];
    
    /* Solicita o nome do jogador */
    printf("=== CAMPO MINADO ===\n");
    printf("Nome do jogador: ");
    if (!fgets(player, sizeof(player), stdin)) return 0;
    /* Remove caractere de nova linha */
    player[strcspn(player, "\r\n")] = '\0';
    if (strlen(player) == 0) strncpy(player, "Anonimo", sizeof(player)-1);

    /* Configuracao do tabuleiro */
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

    /* Cria o tabuleiro */
    Board *b = board_create(rows, cols, bombs);
    if (!b) { fprintf(stderr, "Erro ao alocar tabuleiro\n"); return 1; }

    int total_safe = b->rows * b->cols - b->bombs;
    int running = 1;
    int abandoned = 0;
    
    printf("\nBoa sorte, %s!\n\n", player);
    printf("\nBoa sorte, %s!\n\n", player);
    
    /* Loop principal do jogo */
    while (running) {
        /* Mostra o tabuleiro e o escore atual */
        board_print(b, 0);
        double current_score = calculate_score(b->revealed_count, total_safe);
        printf("\nJogador: %s | Escore atual: %.2f | Casas reveladas: %d/%d\n", 
               player, current_score, b->revealed_count, total_safe);
        
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
                    /* Detonou uma bomba - fim de jogo */
                    board_reveal_all_bombs(b);
                    board_print(b, 1);
                    printf("\nBOOM! Voce detonou uma bomba. Fim de jogo.\n");
                    running = 0;
                } else if (res == -1) {
                    printf("Casa ja revelada ou marcada\n");
                } else {
                    /* Verifica se venceu o jogo */
                    if (b->revealed_count == total_safe) {
                        board_print(b, 1);
                        printf("\n*** PARABENS %s! VOCE VENCEU! ***\n", player);
                        double esc = calculate_score(b->revealed_count, total_safe);
                        printf("Seu escore final: %.2f\n", esc);
                        try_add_score(RANKING_FILE, player, esc);
                        printf("Escore adicionado ao ranking!\n");
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
