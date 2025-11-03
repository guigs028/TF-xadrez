#include "board.h"
#include <string.h>
#include <time.h>

static int dr[8] = {-1,-1,-1,0,0,1,1,1};
static int dc[8] = {-1,0,1,-1,1,-1,0,1};

Board *board_create(int rows, int cols, int bombs) {
    Board *b = malloc(sizeof(Board));
    if (!b) return NULL;
    b->rows = rows;
    b->cols = cols;
    b->bombs = bombs;
    b->revealed_count = 0;
    b->first_move_done = 0;
    int cells = rows * cols;
    b->bomb = calloc(cells, 1);
    b->revealed = calloc(cells, 1);
    b->flag = calloc(cells, 1);
    b->adj = calloc(cells, sizeof(int));
    if (!b->bomb || !b->revealed || !b->flag || !b->adj) {
        board_free(b);
        return NULL;
    }
    return b;
}

void board_free(Board *b) {
    if (!b) return;
    free(b->bomb);
    free(b->revealed);
    free(b->flag);
    free(b->adj);
    free(b);
}

int board_index(Board *b, int r, int c) {
    return r * b->cols + c;
}

int board_in_bounds(Board *b, int r, int c) {
    return r >= 0 && r < b->rows && c >= 0 && c < b->cols;
}

int board_is_bomb(Board *b, int r, int c) {
    return b->bomb[board_index(b,r,c)];
}

int board_is_revealed(Board *b, int r, int c) {
    return b->revealed[board_index(b,r,c)];
}

int board_is_flagged(Board *b, int r, int c) {
    return b->flag[board_index(b,r,c)];
}

void board_place_bombs(Board *b, int avoid_r, int avoid_c) {
    // place bombs randomly, avoid the cell (avoid_r, avoid_c)
    int cells = b->rows * b->cols;
    int placed = 0;
    srand((unsigned)time(NULL));
    while (placed < b->bombs) {
        int idx = rand() % cells;
        int r = idx / b->cols;
        int c = idx % b->cols;
        if (r == avoid_r && c == avoid_c) continue;
        if (b->bomb[idx]) continue;
        b->bomb[idx] = 1;
        placed++;
    }
    board_compute_adjacency(b);
    b->first_move_done = 1;
}

void board_compute_adjacency(Board *b) {
    memset(b->adj, 0, sizeof(int) * b->rows * b->cols);
    for (int r = 0; r < b->rows; ++r) {
        for (int c = 0; c < b->cols; ++c) {
            int idx = board_index(b,r,c);
            if (b->bomb[idx]) continue;
            int count = 0;
            for (int k=0;k<8;k++){
                int rr = r + dr[k];
                int cc = c + dc[k];
                if (board_in_bounds(b, rr, cc) && b->bomb[board_index(b,rr,cc)]) count++;
            }
            b->adj[idx] = count;
        }
    }
}

static void reveal_dfs(Board *b, int r, int c) {
    if (!board_in_bounds(b,r,c)) return;
    int idx = board_index(b,r,c);
    if (b->revealed[idx] || b->flag[idx]) return;
    b->revealed[idx] = 1;
    b->revealed_count++;
    if (b->adj[idx] != 0) return;
    for (int k=0;k<8;k++){
        int rr = r + dr[k];
        int cc = c + dc[k];
        if (board_in_bounds(b, rr, cc)) {
            int idx2 = board_index(b,rr,cc);
            if (!b->revealed[idx2] && !b->bomb[idx2]) reveal_dfs(b, rr, cc);
        }
    }
}

// returns: 0 okay, 1 bomb detonated, -1 already revealed
int board_reveal(Board *b, int r, int c) {
    if (!board_in_bounds(b,r,c)) return 0;
    if (!b->first_move_done) {
        board_place_bombs(b, r, c);
        // ensure adjacency computed
    }
    int idx = board_index(b,r,c);
    if (b->revealed[idx]) return -1;
    if (b->flag[idx]) return -1;
    if (b->bomb[idx]) {
        b->revealed[idx] = 1;
        b->revealed_count++;
        return 1; // detonated
    }
    reveal_dfs(b, r, c);
    return 0;
}

void board_toggle_flag(Board *b, int r, int c) {
    if (!board_in_bounds(b,r,c)) return;
    int idx = board_index(b,r,c);
    if (b->revealed[idx]) return; // can't flag revealed
    b->flag[idx] = b->flag[idx] ? 0 : 1;
}

void board_reveal_all_bombs(Board *b) {
    for (int i=0;i<b->rows * b->cols;i++) {
        if (b->bomb[i]) b->revealed[i] = 1;
    }
}

void board_print(Board *b, int show_bombs) {
    // column headers
    printf("   ");
    for (int c=0;c<b->cols;c++) printf(" %d", c);
    printf("\n");
    for (int r=0;r<b->rows;r++){
        printf("%2d ", r);
        for (int c=0;c<b->cols;c++){
            int idx = board_index(b,r,c);
            if (b->revealed[idx]){
                if (b->bomb[idx]) printf(" *");
                else if (b->adj[idx] == 0) printf("  ");
                else printf(" %d", b->adj[idx]);
            } else if (b->flag[idx]){
                printf(" +");
            } else {
                if (show_bombs && b->bomb[idx]) printf(" *");
                else printf(" X");
            }
        }
        printf("\n");
    }
}
