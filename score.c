#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int cmp_desc(const void *a, const void *b) {
    const Entry *ea = a;
    const Entry *eb = b;
    if (ea->score < eb->score) return 1;
    if (ea->score > eb->score) return -1;
    return 0;
}

void load_scores(const char *filename, Entry list[], int *n) {
    *n = 0;
    FILE *f = fopen(filename, "r");
    if (!f) return;
    while (*n < MAX_SCORES) {
        if (fscanf(f, "%63s %lf", list[*n].name, &list[*n].score) != 2) break;
        (*n)++;
    }
    fclose(f);
}

void save_scores(const char *filename, Entry list[], int n) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    for (int i=0;i<n;i++) fprintf(f, "%s %.6f\n", list[i].name, list[i].score);
    fclose(f);
}

void try_add_score(const char *filename, const char *name, double score) {
    Entry list[MAX_SCORES+1];
    int n = 0;
    load_scores(filename, list, &n);
    // append new
    strncpy(list[n].name, name, MAX_NAME-1);
    list[n].name[MAX_NAME-1] = '\0';
    list[n].score = score;
    n++;
    // sort desc
    qsort(list, n, sizeof(Entry), cmp_desc);
    if (n > MAX_SCORES) n = MAX_SCORES;
    save_scores(filename, list, n);
}

void print_scores(const char *filename) {
    Entry list[MAX_SCORES];
    int n = 0;
    load_scores(filename, list, &n);
    printf("\n== Ranking (top %d) ==\n", MAX_SCORES);
    for (int i=0;i<n;i++) {
        printf("%2d. %-20s %.2f\n", i+1, list[i].name, list[i].score);
    }
    if (n==0) printf("(sem registros)\n");
    printf("====================\n\n");
}
