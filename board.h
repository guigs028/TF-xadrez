/**
 * MODULO BOARD (Tabuleiro)
 * Define a estrutura do tabuleiro e funcoes para manipula-lo
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Estrutura que representa o tabuleiro do jogo
 */
typedef struct {
    int rows;              /* Numero de linhas */
    int cols;              /* Numero de colunas */
    int bombs;             /* Numero de bombas */
    int revealed_count;    /* Contador de casas reveladas */
    int first_move_done;   /* Flag: primeira jogada ja feita? */
    char *bomb;            /* Array: 1 se tem bomba, 0 caso contrario */
    char *revealed;        /* Array: 1 se revelada, 0 caso contrario */
    char *flag;            /* Array: 1 se marcada com bandeira, 0 caso contrario */
    int *adj;              /* Array: numero de bombas adjacentes */
} Board;

Board *board_create(int rows, int cols, int bombs);
void board_free(Board *b);
void board_place_bombs(Board *b, int avoid_r, int avoid_c);
void board_compute_adjacency(Board *b);
int board_in_bounds(Board *b, int r, int c);
int board_index(Board *b, int r, int c);
int board_is_bomb(Board *b, int r, int c);
int board_is_revealed(Board *b, int r, int c);
int board_is_flagged(Board *b, int r, int c);
void board_toggle_flag(Board *b, int r, int c);
int board_reveal(Board *b, int r, int c);
void board_reveal_all_bombs(Board *b);
void board_print(Board *b, int show_bombs);

#endif // BOARD_H
