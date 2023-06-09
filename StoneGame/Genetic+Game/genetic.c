#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "genetic.h"

void init_pop(Genetic *genetic, Player *player)
{
    genetic->population = malloc(genetic->n_pop * sizeof(Player));
    for (int i = 0; i < genetic->n_pop; i++)
    {
        genetic->population[i] = player[i];

        player[i].points = 0;
        player[i].genes = malloc(genetic->gens_len * sizeof(double));
        genetic->population[i].genes = malloc(genetic->gens_len * sizeof(double));
        genetic->population[i].points = player[i].points;
        for (int j = 0; j < genetic->gens_len; j++)
        {
            player[i].genes[j] = (rand() / (double)RAND_MAX);
            genetic->population[i].genes[j] = player[i].genes[j];
        }
    }
}

void calc_scores(Genetic *genetic)
{
    genetic->scores = malloc(genetic->n_pop * sizeof(int));
    for (int i = 0; i < genetic->n_pop; i++)
    {
        genetic->scores[i] = genetic->population[i].points;
    }
}

double *selection(Genetic *genetic)
{
    int selected_i = rand() % (genetic->n_pop - 1) + 1;
    for (int i = 0; i < genetic->n_pop; i++)
    {
        if (genetic->scores[i] > genetic->scores[selected_i])
        {
            selected_i = i;
        }
    }

    return genetic->population[selected_i].genes;
}

double **crossover(Genetic *genetic, double *parent1, double *parent2)
{
    double *child1 = malloc(genetic->gens_len * sizeof(double));
    double *child2 = malloc(genetic->gens_len * sizeof(double));

    for (int i = 0; i < genetic->gens_len; i++)
    {
        child1[i] = parent1[i];
        child2[i] = parent2[i];
    }

    int point = rand() % (genetic->gens_len - 1) + 1;
    for (int i = point; i < genetic->gens_len; i++)
    {
        double temp = child1[i];
        child1[i] = child2[i];
        child2[i] = temp;
    }

    double **childrens = malloc(2 * sizeof(double *));
    childrens[0] = child1;
    childrens[1] = child2;

    return childrens;
}

double *mutation(Genetic *genetic, double *child)
{
    /* TODO: MUTATION LOGIC */
    return 0;
}

void evolve(Genetic *genetic)
{
    genetic->calc_scores(genetic);

    int retain_perc = (int)(genetic->retain_count * genetic->n_pop);
    for (int i = retain_perc; i < genetic->n_pop; i++)
    {
        if ((float)rand() / RAND_MAX < genetic->c_rate)
        {
            double *parent1 = genetic->selection(genetic);
            double *parent2 = genetic->selection(genetic);
            double **children = genetic->crossover(genetic, parent1, parent2);

            int choice = rand() % 2;
            if (choice == 0)
            {
                for (int j = 0; j < genetic->gens_len; j++)
                {
                    genetic->population[i].genes[j] = children[0][j];
                }
            }
            else
            {
                for (int j = 0; j < genetic->gens_len; j++)
                {
                    genetic->population[i].genes[j] = children[1][j];
                }
            }
        }
    }

    for (int i = 0; i < genetic->n_pop; i++)
    {
        genetic->population[i].points = 0;
    }
}

Genetic *create_genetic(int n_pop, int genes_len, float retain_count, float c_rate, float m_rate)
{
    Genetic *genetic = malloc(sizeof(Genetic));
    genetic->n_pop = n_pop;
    genetic->gens_len = genes_len;
    genetic->retain_count = retain_count;
    genetic->c_rate = c_rate;
    genetic->m_rate = m_rate;
    genetic->self = genetic;
    genetic->init_pop = init_pop;
    genetic->calc_scores = calc_scores;
    genetic->selection = selection;
    genetic->crossover = crossover;
    genetic->mutation = mutation;
    genetic->evolve = evolve;

    return genetic;
}

void destroy_genetic(Genetic *genetic)
{
    free(genetic->population);
    free(genetic->scores);
    free(genetic);
}