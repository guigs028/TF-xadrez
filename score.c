#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *funcao de comparacao para ordenar escores em ordem DECRESCENTE
 *(maior escore = melhor, conforme especificacao do trabalho)
 *retorna: positivo se a < b, negativo se a > b, 0 se iguais
 */
static int cmp_desc(const void *a, const void *b) {
    const Entry *ea = a;
    const Entry *eb = b;
    if (ea->score < eb->score) return 1;
    if (ea->score > eb->score) return -1;
    return 0;
}

/**
 *carrega os escores do arquivo de ranking
 * @param filename nome do arquivo a ser lido
 * @param list array onde os escores serao armazenados
 * @param n ponteiro para o numero de escores lidos
 */
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

/**
 *salva os escores no arquivo de ranking
 * @param filename nome do arquivo onde salvar
 * @param list array com os escores a serem salvos
 * @param n numero de escores a salvar
 */
void save_scores(const char *filename, Entry list[], int n) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    for (int i=0;i<n;i++) fprintf(f, "%s %.6f\n", list[i].name, list[i].score);
    fclose(f);
}

/**
 *tenta adicionar um novo escore ao ranking
 *carrega o ranking existente, adiciona o novo escore,
 *ordena em ordem decrescente (maior = melhor) e salva apenas os top 10
 * @param filename nome do arquivo de ranking
 * @param name nome do jogador
 * @param score escore obtido
 */
void try_add_score(const char *filename, const char *name, double score) {
    Entry list[MAX_SCORES+1];
    int n = 0;
    load_scores(filename, list, &n);
    //adiciona novo escore
    strncpy(list[n].name, name, MAX_NAME-1);
    list[n].name[MAX_NAME-1] = '\0';
    list[n].score = score;
    n++;
    //ordena em ordem decrescente (maiores escores primeiro = melhores)
    qsort(list, n, sizeof(Entry), cmp_desc);
    if (n > MAX_SCORES) n = MAX_SCORES;
    save_scores(filename, list, n);
}

/**
 *exibe o ranking dos 10 melhores escores na tela
 * @param filename nome do arquivo de ranking
 */
void print_scores(const char *filename) {
    Entry list[MAX_SCORES];
    int n = 0;
    load_scores(filename, list, &n);
    printf("\n========== RANKING (Top %d) ==========\n", MAX_SCORES);
    for (int i=0;i<n;i++) {
        printf("%2d. %-20s %.2f\n", i+1, list[i].name, list[i].score);
    }
    if (n==0) printf("(Nenhum registro ainda)\n");
    printf("======================================\n\n");
}
