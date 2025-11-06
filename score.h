/**
 *MODULO SCORE (Placar/Ranking)
 *gerencia o arquivo de recordes com os 10 melhores escores
 *maior escore = melhor (mais casas reveladas)
 */

#ifndef SCORE_H
#define SCORE_H

#define MAX_NAME 64      //tamanho maximo do nome do jogador
#define MAX_SCORES 10    //numero maximo de escores no ranking


//estrutura que representa uma entrada no ranking
typedef struct {
    char name[MAX_NAME];  //nome do jogador
    double score;         //escore obtido
} Entry;

void load_scores(const char *filename, Entry list[], int *n);
void save_scores(const char *filename, Entry list[], int n);
void try_add_score(const char *filename, const char *name, double score);
void print_scores(const char *filename);

#endif //SCORE_H
