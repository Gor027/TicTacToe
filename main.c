#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * int board[25] = {
 *   3,3,3,3,3,
 *   3,-,-,-,3,
 *   3,-,-,-,3,
 *   3,-,-,-,3,
 *   3,3,3,3,3,
 * }
 * */

enum {
    NAUGHTS, CROSSES, BORDER, EMPTY
};

const int ConvertTo25[9] = {
        6, 7, 8,
        11, 12, 13,
        16, 17, 18
};

const int DIR[4] = {1, 5, 4, 6};

const int CENTER = 4;
const int Corners[4] = {0, 2, 6, 8};


void initializeBoard(int *board) {

    for (int i = 0; i < 25; ++i) {
        board[i] = BORDER;
    }

    for (int i = 0; i < 9; ++i) {
        board[ConvertTo25[i]] = EMPTY;
    }
}

void printBoard(const int *board) {

    char pceChars[] = "OX|-";
    printf("\n\nBoard:\n");

    for (int i = 0; i < 9; ++i) {
        if (i != 0 && i % 3 == 0) {
            printf("\n\n");
        }
        printf("%4c", pceChars[board[ConvertTo25[i]]]);

    }
    printf("\n");

}

int hasEmpty(const int *board) {

    for (int i = 0; i < 9; ++i) {
        if (board[ConvertTo25[i]] == EMPTY) {
            return 1;
        }
    }
    return 0;

}

int getNumForDir(int start, int dir, const int *board, const int side) {

    int found = 0;
    while (board[start] != BORDER) {

        if (board[start] != side) {
            break;
        }

        found++;
        start += dir;
    }

    return found;

}

int findThreeCons(const int *board, int start, const int side) {

    int dir = 0;
    int threeCount = 1;

    for (int i = 0; i < 4; ++i) {

        dir = DIR[i];

        threeCount += getNumForDir(start + dir, dir, board, side);
        threeCount += getNumForDir(start + dir * -1, dir * -1, board, side);

        if (threeCount == 3) {
            break;
        }
        threeCount = 1;
    }

    return threeCount;

}

void makeMove(int *board, int sq, int side) {
    board[sq] = side;
}


// com finds basic win move
int findWinMove(int *board, const int side) {

    int comMove = -1;
    int winFound = 0;

    for (int i = 0; i < 9; ++i) {

        if (board[ConvertTo25[i]] == EMPTY) {

            comMove = ConvertTo25[i];
            board[comMove] = side; // assign side to check findThreeCons

            if (findThreeCons(board, comMove, side) == 3) {
                winFound = 1;
            }
            board[comMove] = EMPTY;

            if (winFound == 1) {
                break;
            }

            comMove = -1;
        }

    }
    return comMove;
}

// move in corner or corners
int findBetterMove(const int *board) {

    int comMove = ConvertTo25[CENTER];

    if (board[comMove] == EMPTY) {
        return comMove;
    }

    comMove = -1;
    for (int i = 0; i < 4; ++i) {
        comMove = ConvertTo25[Corners[i]];
        if (board[comMove] == EMPTY) {
            break;
        }
        comMove = -1;
    }

    return comMove;
}

int getComMove(int *board, const int side) {
    int availableMoves[9];
    int numFree = 0;
    int move = 0;

    move = findWinMove(board, side); // Win move
    if (move != -1) {
        return move;
    }

    move = findWinMove(board, side ^ 1); // Blocks user win
    if (move != -1) {
        return move;
    }

    move = findBetterMove(board); // Does better move
    if (move != -1) {
        return move;
    }

    for (int i = 0; i < 9; ++i) {
        if (board[ConvertTo25[i]] == EMPTY) {
            availableMoves[numFree++] = ConvertTo25[i];
        }
    }

    move = (rand() % numFree);
    return availableMoves[move];
}

int getPlrMove(const int *board) {

    char usrInput[4];

    int moveOK = 0;
    int move = -1;

    while (moveOK == 0) {

        printf("Enter your move in range of 1 to 9 ...\n");
        fgets(usrInput, 10, stdin);
        fflush(stdin);

        if (strlen(usrInput) != 2) {
            printf("Invalid length\n");
            continue;
        }

        if (sscanf(usrInput, "%d", &move) != 1) { //usrInput is stored in move
            move = -1;
            printf("Invalid input...\n");
            continue;
        }

        if (move < 1 || move > 9) {
            move = -1;
            printf("Invalid range ...\n");
            continue;
        }

        if (board[ConvertTo25[move - 1]] != EMPTY) {
            move = -1;
            printf("Square is not available ...\n");
            continue;
        }

        moveOK = 1;
    }
    printf("Made the move ... %d\n", move);
    return ConvertTo25[move - 1];

}

void runGame() {

    int gameOver = 0;
    int side = CROSSES;
    int lastMoveMade = 0;

    int board[25];
    initializeBoard(board);
    printBoard(board);

    while (!gameOver) {
        if (side == CROSSES) {
            // get move from player, make the move, change side
            lastMoveMade = getPlrMove(board);
            makeMove(board, lastMoveMade, side);
            side = NAUGHTS;

        } else {
            // get move from comp, make the move, change side
            lastMoveMade = getComMove(board, side);
            makeMove(board, lastMoveMade, side);
            side = CROSSES;

            //print the result
            printBoard(board);
        }

        // If three in a series, Game Over
        if (findThreeCons(board, lastMoveMade, side ^ 1) == 3) {
            printf("Game Over!\n");
            gameOver = 1;
            if (side == NAUGHTS) {
                printf("User Wins!\n");
            } else {
                printf("Computer Wins!\n");
            }
        }

        // if no more place on board, DRAW
        if (!hasEmpty(board)) {
            printf("Game Over!\n");
            gameOver = 1;
            printf("It is a DRAW!\n");
        }
    }

    printBoard(board);

}


int main() {

    srand(time(NULL));
    runGame();

    return 0;
}