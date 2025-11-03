/**
 * MODULO SCORE (Placar/Ranking)
 * Gerencia o arquivo de recordes com os 10 melhores escores
 * Menor escore = melhor (mais eficiente)
 */

#ifndef SCORE_H
#define SCORE_H

#define MAX_NAME 64      /* Tamanho maximo do nome do jogador */
#define MAX_SCORES 10    /* Numero maximo de escores no ranking */

/**
 * Estrutura que representa uma entrada no ranking
 */
typedef struct {
    char name[MAX_NAME];  /* Nome do jogador */
    double score;         /* Escore obtido */
} Entry;

void load_scores(const char *filename, Entry list[], int *n);
void save_scores(const char *filename, Entry list[], int n);
void try_add_score(const char *filename, const char *name, double score);
void print_scores(const char *filename);

#endif // SCORE_H
