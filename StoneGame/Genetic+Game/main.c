#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "genetic.c"

#define BOARD_SIZE 8

void show_board(char board[BOARD_SIZE][BOARD_SIZE]);
void make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol, int distanceX, int distanceY, Player *player);
void make_board(char board[BOARD_SIZE][BOARD_SIZE]);
void make_player(Player *player, char board[BOARD_SIZE][BOARD_SIZE]);

int main()
{
    srand(time(NULL));

    FILE *file;
    char board[BOARD_SIZE][BOARD_SIZE];
    int win = 0;
    int n_pop = 100;
    int gens_len = 9;
    float retain_count = 0.2;
    float c_rate = 0.1;
    float m_rate = 0.1;
    int generation = 0;
    int max_generation = 100;
    int matches = 50;

    Genetic *genetic = create_genetic(n_pop, gens_len, retain_count, c_rate, m_rate);
    Player *players = malloc(n_pop * sizeof(Player));

    genetic->init_pop(genetic, players);

    while (generation < max_generation)
    {
        /*
            List of Genes:
                Gene1:  Choose stone x
                Gene2:  Choose stone y
                Gene3:  Choose stone target x
                Gene4:  Choose stone target y
                Gene5:  Preference for horizontal movement
                Gene6:  Preference for vertical movement
                Gene7:  Stone eating preference
                Gene8:  Defensive play
                Gene9:  Randomness
        */

        for (int i = 0; i < n_pop - 1; i += 2)
        {
            make_board(board);
            for (int j = 0; j < matches; j++)
            {
                make_player(&genetic->population[i], board);
                make_player(&genetic->population[i + 1], board);
            }
        }

        int points = 0;
        for (int i = 0; i < n_pop; i++)
        {
            points += genetic->population[i].points;
        }

        printf("Mean of points of generation %d: %d\n", generation + 1, points / n_pop);

        genetic->evolve(genetic);

        generation += 1;
    }

    show_board(board);

    file = fopen("genes.json", "w");
    for (int i = 0; i < gens_len; i++)
    {
        fprintf(file, "%lf ", genetic->population[n_pop - 1].genes[i]);
    }
    fclose(file);
}

void make_board(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            if (col > 1 && col < 6 && row > 1 && row < 6)
            {
                board[col][row] = '-';
            }
            else
            {
                board[col][row] = '*';
            }
        }
    }
}

void show_board(char board[BOARD_SIZE][BOARD_SIZE])
{
    printf("\n");
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            printf("%c ", board[col][row]);
        }
        printf("\n");
    }
    printf("\n");
}

void make_player(Player *player, char board[BOARD_SIZE][BOARD_SIZE])
{
    player->genes[8] = (rand() / (double)RAND_MAX);
    int stoneRow = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[0]);
    int stoneCol = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[1]);
    int targetRow = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[2]);
    int targetCol = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[3]);

    int distanceX = targetRow - stoneRow;
    int distanceY = targetCol - stoneCol;

    int getter = 0;
    while (board[stoneCol][stoneRow] == '-' || board[targetCol][targetRow] == '*')
    {
        player->genes[8] = (rand() / (double)RAND_MAX);
        stoneRow = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[0]);
        stoneCol = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[1]);
        targetRow = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[2]);
        targetCol = (int)((BOARD_SIZE - 1) * player->genes[8] * player->genes[3]);

        distanceX = targetRow - stoneRow;
        distanceY = targetCol - stoneCol;

        if (getter > 10)
        {
            stoneRow = rand() % BOARD_SIZE;
            stoneCol = rand() % BOARD_SIZE;
            targetRow = rand() % BOARD_SIZE;
            targetCol = rand() % BOARD_SIZE;
        }

        getter += 1;
    }

    make_move(board, stoneRow, stoneCol, targetRow, targetCol, distanceX, distanceY, player);
}

void make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol, int distanceX, int distanceY, Player *player)
{
    int stepX, stepY;
    if (distanceY < 0)
        stepY = -1;
    else
        stepY = 1;
    if (distanceX < 0)
        stepX = -1;
    else
        stepX = 1;

    if (abs(distanceX) == abs(distanceY))
    {
        for (int y = col + stepY; (distanceY < 0) ? (y > targetCol) : (y < targetCol); y += stepY)
        {
            if (board[y][row + stepX] == '*')
            {
                player->points += 1;
                board[y][row + stepX] = '-';
            }
            stepX += (distanceX < 0) ? -1 : 1;
        }
    }

    board[targetCol][targetRow] = '*';
    board[col][row] = '-';
}