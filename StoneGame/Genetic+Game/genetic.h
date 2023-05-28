#ifndef GENETIC_H
#define GENETIC_H

typedef struct Player
{
    double *genes;
    int points;
} Player;

typedef struct genetic
{
    int n_pop;
    int gens_len;
    float retain_count;
    float c_rate;
    float m_rate;
    Player *population;
    int *scores;
    struct genetic *self;
    void (*init_pop)(struct genetic *genetic, Player *player);
    void (*calc_scores)(struct genetic *genetic);
    double *(*selection)(struct genetic *genetic);
    double **(*crossover)(struct genetic *genetic, double *parent1, double *parent2);
    double *(*mutation)(struct genetic *genetic, double *child);
    void (*evolve)(struct genetic *genetic);
} Genetic;

Genetic *create_genetic(int n_pop, int genes_len, float retain_count, float c_rate, float m_rate);
void destroy_genetic(Genetic *genetic);

#endif