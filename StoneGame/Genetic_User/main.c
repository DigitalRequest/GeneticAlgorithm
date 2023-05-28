#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "genetic.c"

#define BOARD_SIZE 8

void show_board(char board[BOARD_SIZE][BOARD_SIZE]);
void make_move_ai(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol, int distanceX, int distanceY, Player *player);
void make_move_player(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol);
void make_board(char board[BOARD_SIZE][BOARD_SIZE]);
void make_player(Player *player, char board[BOARD_SIZE][BOARD_SIZE]);

int main()
{
    srand(time(NULL));

    char board[BOARD_SIZE][BOARD_SIZE];
    int selectedRow, selectedCol;
    int targetRow, targetCol;
    int gens_len = 9;
    int win = 0;

    Player player;
    player.points = 0;
    double genes[] = {0.057070, 0.376385, 0.135990, 0.372143, 0.616199, 0.033876, 0.809381, 0.463179, 0.323130};
    for (int i = 0; i < gens_len; i++)
    {
        player.genes[i] = genes[i];
    }

    printf("For starting principles, that's the initial board:\n");
    make_board(board);
    show_board(board);
    while (win == 0)
    {
        make_player(&player, board);
        show_board(board);

        printf("Now, we need to get your input\n");
        printf("Please select one row in the board: ");
        scanf("%d", &selectedRow);
        while (selectedRow < 1 || selectedRow > 8)
        {
            printf("ERROR: Value out of bounds in board\n");
            printf("Please select one row in the board: ");
            scanf("%d", &selectedRow);
        }

        printf("Now, please select one colum in the board: ");
        scanf("%d", &selectedCol);
        while (selectedCol < 1 || selectedCol > 8)
        {
            printf("ERROR: Value out of bounds in board\n");
            printf("Now, please select one colum in the board: ");
            scanf("%d", &selectedCol);
        }

        printf("Now, we need to get your input\n");
        printf("Please select the target row in the board: ");
        scanf("%d", &targetRow);
        while (targetRow < 1 || targetRow > 8)
        {
            printf("ERROR: Value out of bounds in board\n");
            printf("Please select the target row in the board: ");
            scanf("%d", &targetRow);
        }

        printf("Now, please select the target colum in the board: ");
        scanf("%d", &targetCol);
        while (targetCol < 1 || targetCol > 8)
        {
            printf("ERROR: Value out of bounds in board\n");
            printf("Now, please select the target colum in the board: ");
            scanf("%d", &targetCol);
        }

        selectedRow = selectedRow - 1;
        selectedCol = selectedCol - 1;

        targetRow = targetRow - 1;
        targetCol = targetCol - 1;

        if (board[selectedCol][selectedRow] != '*')
        {
            printf("ERROR: You selected a blank space\n");
        }
        else
        {
            if (board[targetCol][targetRow] != '*')
            {
                make_move_player(board, selectedRow, selectedCol, targetRow, targetCol);
            }
            else
            {
                printf("ERROR: You can only jump to blank spaces!\n");
            }
        }
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

        if (getter > 100)
        {
            stoneRow = rand() % BOARD_SIZE;
            stoneCol = rand() % BOARD_SIZE;
            targetRow = rand() % BOARD_SIZE;
            targetCol = rand() % BOARD_SIZE;
        }

        getter += 1;
    }

    make_move_ai(board, stoneRow, stoneCol, targetRow, targetCol, distanceX, distanceY, player);
}

void make_move_ai(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol, int distanceX, int distanceY, Player *player)
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
                printf("EATED ONE STONE!!!\n");
            }
            printf("STEPX: %d\n", stepX);
            stepX += (distanceX < 0) ? -1 : 1;
        }
    }
    board[targetCol][targetRow] = '*';
    board[col][row] = '-';

    printf("Jumped from (%d,%d) to (%d,%d), Points: %d\n", row + 1, col + 1, targetRow + 1, targetCol + 1, player->points);
}

void make_move_player(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol)
{
    int distanceX = targetRow - row;
    int distanceY = targetCol - col;
    int stones_eaten = 0;

    if (abs(distanceX) == abs(distanceY))
    {
        int stepY = distanceY / abs(distanceY);
        int stepX = distanceX / abs(distanceX);

        for (int y = col + stepY; y < targetCol || y > targetCol; y += stepY)
        {
            if (board[y][row + stepX] == '*')
            {
                board[y][row + stepX] = '-';
                stones_eaten += 1;
            }
            stepX += distanceX / abs(distanceX);
        }
    }

    board[targetCol][targetRow] = '*';
    board[col][row] = '-';
    printf("The stone jumped from (%d,%d) to (%d,%d), with %d '*'(stone[s]) beeing eliminated\n", row + 1, col + 1, targetRow + 1, targetCol + 1, stones_eaten);
}