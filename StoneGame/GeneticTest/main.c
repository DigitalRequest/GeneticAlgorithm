#include "genetic.c"

int main()
{
    srand(time(NULL));
    int n_pop = 100;
    int gens_len = 5;
    float retain_count = 0.4;
    float c_rate = 0.1;
    float m_rate = 0.1;

    Genetic* genetic = create_genetic(n_pop, gens_len, retain_count, c_rate, m_rate);
    Player* players = malloc(n_pop * sizeof(Player));

    for (int i = 0; i < n_pop; i++)
    {
        players[i].genes = malloc(gens_len * sizeof(double));
        for (int j = 0; j < gens_len; j++)
        {
            players[i].genes[j] = (double) rand() / RAND_MAX;
        }
    }

    genetic->init_pop(genetic, players);
    genetic->evolve(genetic);

    // Clean up memory
    for (int i = 0; i < n_pop; i++) {
        free(players[i].genes);
    }
    free(players);
}