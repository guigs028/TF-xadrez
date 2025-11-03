#include "board.h"
#include <string.h>
#include <time.h>

/* Arrays para calcular as 8 posicoes adjacentes (vizinhos) */
static int dr[8] = {-1,-1,-1,0,0,1,1,1};
static int dc[8] = {-1,0,1,-1,1,-1,0,1};

/**
 * Cria um novo tabuleiro alocando memoria para todas as estruturas
 * @param rows Numero de linhas do tabuleiro
 * @param cols Numero de colunas do tabuleiro
 * @param bombs Numero de bombas a serem colocadas
 * @return Ponteiro para o tabuleiro criado ou NULL em caso de erro
 */
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

/**
 * Libera toda a memoria alocada para o tabuleiro
 * @param b Ponteiro para o tabuleiro a ser liberado
 */
void board_free(Board *b) {
    if (!b) return;
    free(b->bomb);
    free(b->revealed);
    free(b->flag);
    free(b->adj);
    free(b);
}

/**
 * Calcula o indice linear de uma posicao (linha, coluna) no array
 * @param b Ponteiro para o tabuleiro
 * @param r Linha
 * @param c Coluna
 * @return Indice no array unidimensional
 */
int board_index(Board *b, int r, int c) {
    return r * b->cols + c;
}

/**
 * Verifica se uma posicao esta dentro dos limites do tabuleiro
 * @param b Ponteiro para o tabuleiro
 * @param r Linha
 * @param c Coluna
 * @return 1 se esta dentro dos limites, 0 caso contrario
 */
int board_in_bounds(Board *b, int r, int c) {
    return r >= 0 && r < b->rows && c >= 0 && c < b->cols;
}

/**
 * Verifica se uma casa contem uma bomba
 */
int board_is_bomb(Board *b, int r, int c) {
    return b->bomb[board_index(b,r,c)];
}

/**
 * Verifica se uma casa ja foi revelada
 */
int board_is_revealed(Board *b, int r, int c) {
    return b->revealed[board_index(b,r,c)];
}

/**
 * Verifica se uma casa esta marcada com bandeira
 */
int board_is_flagged(Board *b, int r, int c) {
    return b->flag[board_index(b,r,c)];
}

/**
 * Coloca as bombas aleatoriamente no tabuleiro, evitando uma posicao especifica
 * (geralmente a primeira jogada do usuario)
 * @param b Ponteiro para o tabuleiro
 * @param avoid_r Linha a evitar
 * @param avoid_c Coluna a evitar
 */
void board_place_bombs(Board *b, int avoid_r, int avoid_c) {
    int cells = b->rows * b->cols;
    int placed = 0;
    srand((unsigned)time(NULL));
    while (placed < b->bombs) {
        int idx = rand() % cells;
        int r = idx / b->cols;
        int c = idx % b->cols;
        /* Nao coloca bomba na posicao que o jogador escolheu primeiro */
        if (r == avoid_r && c == avoid_c) continue;
        /* Nao coloca bomba em posicao que ja tem bomba */
        if (b->bomb[idx]) continue;
        b->bomb[idx] = 1;
        placed++;
    }
    board_compute_adjacency(b);
    b->first_move_done = 1;
}

/**
 * Calcula o numero de bombas adjacentes para cada casa do tabuleiro
 * @param b Ponteiro para o tabuleiro
 */
void board_compute_adjacency(Board *b) {
    memset(b->adj, 0, sizeof(int) * b->rows * b->cols);
    for (int r = 0; r < b->rows; ++r) {
        for (int c = 0; c < b->cols; ++c) {
            int idx = board_index(b,r,c);
            if (b->bomb[idx]) continue;
            int count = 0;
            /* Conta bombas nas 8 posicoes adjacentes */
            for (int k=0;k<8;k++){
                int rr = r + dr[k];
                int cc = c + dc[k];
                if (board_in_bounds(b, rr, cc) && b->bomb[board_index(b,rr,cc)]) count++;
            }
            b->adj[idx] = count;
        }
    }
}

/**
 * Funcao recursiva (DFS) para revelar casas vazias e suas adjacentes
 * Quando uma casa sem bombas adjacentes e revelada, todas as casas
 * ao redor tambem sao reveladas automaticamente
 * @param b Ponteiro para o tabuleiro
 * @param r Linha
 * @param c Coluna
 */
static void reveal_dfs(Board *b, int r, int c) {
    if (!board_in_bounds(b,r,c)) return;
    int idx = board_index(b,r,c);
    if (b->revealed[idx] || b->flag[idx]) return;
    b->revealed[idx] = 1;
    b->revealed_count++;
    /* Se a casa tem bombas adjacentes, para a recursao */
    if (b->adj[idx] != 0) return;
    /* Casa vazia: revela todas as adjacentes recursivamente */
    for (int k=0;k<8;k++){
        int rr = r + dr[k];
        int cc = c + dc[k];
        if (board_in_bounds(b, rr, cc)) {
            int idx2 = board_index(b,rr,cc);
            if (!b->revealed[idx2] && !b->bomb[idx2]) reveal_dfs(b, rr, cc);
        }
    }
}

/**
 * Revela uma casa do tabuleiro
 * @param b Ponteiro para o tabuleiro
 * @param r Linha
 * @param c Coluna
 * @return 0 se ok, 1 se detonou bomba, -1 se casa ja revelada/marcada
 */
int board_reveal(Board *b, int r, int c) {
    if (!board_in_bounds(b,r,c)) return 0;
    /* Se for a primeira jogada, coloca as bombas evitando esta posicao */
    if (!b->first_move_done) {
        board_place_bombs(b, r, c);
    }
    int idx = board_index(b,r,c);
    if (b->revealed[idx]) return -1;
    if (b->flag[idx]) return -1;
    if (b->bomb[idx]) {
        b->revealed[idx] = 1;
        b->revealed_count++;
        return 1; /* Detonou bomba - fim de jogo */
    }
    reveal_dfs(b, r, c);
    return 0;
}

/**
 * Marca ou desmarca uma casa com bandeira
 * @param b Ponteiro para o tabuleiro
 * @param r Linha
 * @param c Coluna
 */
void board_toggle_flag(Board *b, int r, int c) {
    if (!board_in_bounds(b,r,c)) return;
    int idx = board_index(b,r,c);
    if (b->revealed[idx]) return; /* Nao pode marcar casa revelada */
    b->flag[idx] = b->flag[idx] ? 0 : 1;
}

/**
 * Revela todas as bombas do tabuleiro (usado no fim do jogo)
 * @param b Ponteiro para o tabuleiro
 */
void board_reveal_all_bombs(Board *b) {
    for (int i=0;i<b->rows * b->cols;i++) {
        if (b->bomb[i]) b->revealed[i] = 1;
    }
}

/**
 * Imprime o tabuleiro na tela
 * @param b Ponteiro para o tabuleiro
 * @param show_bombs Se 1, revela todas as bombas (usado no fim do jogo)
 */
void board_print(Board *b, int show_bombs) {
    /* Cabecalho com numeros das colunas */
    printf("   ");
    for (int c=0;c<b->cols;c++) printf(" %d", c);
    printf("\n");
    /* Imprime cada linha do tabuleiro */
    for (int r=0;r<b->rows;r++){
        printf("%2d ", r);
        for (int c=0;c<b->cols;c++){
            int idx = board_index(b,r,c);
            if (b->revealed[idx]){
                if (b->bomb[idx]) printf(" *");
                else if (b->adj[idx] == 0) printf("  "); /* Casa vazia */
                else printf(" %d", b->adj[idx]);
            } else if (b->flag[idx]){
                printf(" +"); /* Bandeira */
            } else {
                if (show_bombs && b->bomb[idx]) printf(" *");
                else printf(" X"); /* Casa fechada */
            }
        }
        printf("\n");
    }
}
