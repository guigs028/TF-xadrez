#ifndef SCORE_H
#define SCORE_H

#define MAX_NAME 64
#define MAX_SCORES 10

typedef struct {
    char name[MAX_NAME];
    double score;
} Entry;

void load_scores(const char *filename, Entry list[], int *n);
void save_scores(const char *filename, Entry list[], int n);
void try_add_score(const char *filename, const char *name, double score);
void print_scores(const char *filename);

#endif // SCORE_H
