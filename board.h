#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    int bombs;
    int revealed_count;
    int first_move_done;
    char *bomb;     // 1 if bomb, 0 otherwise
    char *revealed; // 1 if revealed, 0 otherwise
    char *flag;     // 1 if flagged, 0 otherwise
    int *adj;       // number of adjacent bombs
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
