#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BOARD_SIZE 8

void show_board(char board[BOARD_SIZE][BOARD_SIZE]);
void make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol);

int main()
{
    char board[BOARD_SIZE][BOARD_SIZE];
    int selectedRow, selectedCol;
    int targetRow, targetCol;
    int win = 0;

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

    printf("For starting principles, that's the initial board:\n");

    while (win == 0)
    {
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
                make_move(board, selectedRow, selectedCol, targetRow, targetCol);
            }
            else
            {
                printf("ERROR: You can only jump to blank spaces!\n");
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

void make_move(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int targetRow, int targetCol)
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
    printf("The stone jumped from (%d,%d) to (%d,%d), with %d '*'(stone[s]) beeing eliminated", row + 1, col + 1, targetRow + 1, targetCol + 1, stones_eaten);
}