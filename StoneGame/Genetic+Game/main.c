#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "genetic.c"

#define BOARD_SIZE 8

void show_board(char board[BOARD_SIZE][BOARD_SIZE]);
void make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol, Player *player);
void make_board(char board[BOARD_SIZE][BOARD_SIZE]);
void make_player(Player *player, char board[BOARD_SIZE][BOARD_SIZE]);
void update_genes(int gens_len, Player *player);

int main()
{
    srand(time(NULL));

    char board[BOARD_SIZE][BOARD_SIZE];
    int win = 0;
    int n_pop = 50;
    int gens_len = 9;
    float retain_count = 0.5;
    float c_rate = 0.1;
    float m_rate = 0.1;
    int generation = 0;
    int max_generation = 10;

    Genetic *genetic = create_genetic(n_pop, gens_len, retain_count, c_rate, m_rate);
    Player *players = malloc(n_pop * sizeof(Player));

    genetic->init_pop(genetic, players);

    make_board(board);

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

        for (int i = 0; i < n_pop - 1; i++)
        {
            make_player(&genetic->population[i], board);
            make_player(&genetic->population[i + 1], board);

            update_genes(gens_len, &genetic->population[i]);
            update_genes(gens_len, &genetic->population[i + 1]);
        }
        
        genetic->evolve(genetic);

        generation += 1;
    }
}

void update_genes(int gens_len, Player *player)
{
    for (int j = 0; j < gens_len; j++)
    {
        player->genes[j] = (rand() / (double)RAND_MAX) * 2;
    }
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
    int stoneRow = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[0], BOARD_SIZE - 1));
    int stoneCol = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[1], BOARD_SIZE - 1));
    int targetRow = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[2] * player->genes[4], BOARD_SIZE - 1));
    int targetCol = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[3] * player->genes[5], BOARD_SIZE - 1));

    while (stoneRow == targetRow && stoneCol == targetCol)
    {
        player->genes[8] = (rand() / (double)RAND_MAX) * 2;
        stoneRow = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[0], BOARD_SIZE - 1));
        stoneCol = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[1], BOARD_SIZE - 1));
        targetRow = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[2] * player->genes[4], BOARD_SIZE - 1));
        targetCol = (int)fmax(0, fmin((BOARD_SIZE - 1) * player->genes[8] * player->genes[3] * player->genes[5], BOARD_SIZE - 1));
    }

    make_move(board, stoneRow, stoneCol, targetRow, targetCol, player);
}

void make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol, Player *player)
{
    int distanceX = targetRow - row;
    int distanceY = targetCol - col;

    if (abs(distanceX) == abs(distanceY))
    {
        int stepY = distanceY / abs(distanceY);
        int stepX = distanceX / abs(distanceX);

        for (int y = col + stepY; y < targetCol || y > targetCol; y += stepY)
        {
            if (board[y][row + stepX] == '*')
            {
                player->points += 1;
                board[y][row + stepX] = '-';
            }
            stepX += distanceX / abs(distanceX);
        }
    }

    board[targetCol][targetRow] = '*';
    board[col][row] = '-';
}