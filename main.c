/**
 * @file main.c
 * @brief A tetris implementation in C
 */

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "sdl.h"
#include "array.h"
#include "events.h"
#include "load_graphics.h"

#define HEIGHT 18
#define WIDTH 12
#define BLK_SIZE 32

#define SCREEN_HEIGHT BLK_SIZE * HEIGHT
#define SCREEN_WIDTH BLK_SIZE * WIDTH

/* "ttm" is an abbreviation for "tetrimino" */
char ITtm[][4] = {
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
};

char LTtm[][4] = {
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 }
};

char JTtm[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 }
};

char TTtm[][4] = {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 1 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
};

char OTtm[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 }
};

char ZTtm[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 1 }
};

char STtm[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 0, 0 }
};

SDL_Color white = { 255, 255, 255, 255 };
SDL_Color black = { 0, 0, 0, 0 };

struct ttm {
    char mat[4][4];
    int row;
    int col;
    int printed;
};

struct ttm curTtm;
char board[HEIGHT][WIDTH] = { 0 };
long score = 0;
SDL_Texture *BLK;
SDL_Renderer *renderer;

#ifdef DEBUG
void printBoard(void) {
    int i;
    int j;

    fprintf(stderr, "\nmatrix:\n");
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            fprintf(stderr, "%i ", board[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "[end]\n\n");
}
#endif

int setBackColor(SDL_Renderer *renderer, SDL_Color color)
{
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
}

/* this function return 1 if it's possible to print the tetrimino and 0 else */
int canPrint(char mat[4][4], int row, int col) {
    int i;
    int j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (mat[i][j] && ( row+i > HEIGHT-1 || col+j > WIDTH-1 || col+j < 0)) { /* if outside board */
                return 0;
            }
            else if (board[row + i][col + j] && mat[i][j]) { /* if there is already another block */
                return 0;
            }
        }
    }
    return 1;
}

int rotateCur(void) {
    char tmp[4][4];
    int i;
    int j;
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tmp[i][j] = curTtm.mat[3 - j][i];
        }
    }

    if (canPrint(tmp, curTtm.row, curTtm.col)) {
        memcpy(curTtm.mat, tmp, 4 * 4 * sizeof(char));
        return 0;
    }
    else {
        return 1;
    }
}

/* this function return 1 if the line is whole and 0 else */
int wholeLine (int row) {
    int i;

    for (i = 0; i < WIDTH; i++)
    {
        if (! board[row][i])
            return 0;
    }
    return 1;
}

int randTtm(void) {
    int choice;
    char (*newTtm)[4];

    curTtm.row = 0;
    curTtm.col = 4;

    choice = rand() % 7;
    switch (choice)
    {
        case 0:
            newTtm = ITtm;
            break;
        case 1:
            newTtm = LTtm;
            break;
        case 2:
            newTtm = JTtm;
            break;
        case 3:
            newTtm = TTtm;
            break;
        case 4:
            newTtm = OTtm;
            break;
        case 5:
            newTtm = STtm;
            break;
        case 6:
            newTtm = ZTtm;
            break;
    }
    memcpy(curTtm.mat, newTtm, 4 * 4 * sizeof(char));

    curTtm.printed = 0;

    if (! canPrint(curTtm.mat, curTtm.row, curTtm.col)) { /* game over! */
        return 1;
    }
    return 0;

}

/**
 * @brief      put a tetrimino in board
 *
 * @param[in]  mat   The matrix of the ttm
 * @param[in]  row   The row in board
 * @param[in]  col   The col in board
 *
 * @return     1 if it's not possible to print the ttm, 0 else
 */
int printTtm(char mat[4][4], int row, int col) {
    int i;
    int j;

    if (! canPrint(mat, row, col))
        return 1;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if ((! board[row + i][col + j]) && mat[i][j]) {
                board[row + i][col + j] = 1;
            }
        }
    }

    return 0;
}

int downTtm(void) {
    curTtm.row++;
    if (! canPrint(curTtm.mat, curTtm.row + 1, curTtm.col)) {
        printTtm(curTtm.mat, curTtm.row, curTtm.col);
        curTtm.printed = 1;
    }
    return 0;
}

void fullDownTtm(void) {
    while (curTtm.printed != 1)
        downTtm();
}

void moveTtm(int direction) {
    if (direction == LEFT) {
        if (canPrint(curTtm.mat, curTtm.row, curTtm.col - 1))
            curTtm.col--;
    }
    else if (direction == RIGHT) {
        if (canPrint(curTtm.mat, curTtm.row, curTtm.col + 1))
            curTtm.col++;
    }
}

void clear(void) {
    int i;
    int j;

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            board[i][j] = 0;
        }
    }
}

void removeLine(int row) {
    int i;
    int j;

    for (i = 0; i < row; i++) {
        for (j = 0; j < WIDTH; j++) {
            board[row - i][j] = board[row-1 - i][j];
        }
    }

    score += 100;
}

/*
@brief remove everything from sreen, refresh with content of \a board array,
show curTtm (at right place)
*/
void refreshScreen(void) {
    int i;
    int j;
    SDL_Rect imgDst = { 0, 0, BLK_SIZE, BLK_SIZE };

    setBackColor(renderer, black);
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            imgDst.x = BLK_SIZE * j;
            imgDst.y = BLK_SIZE * i;

            if (board[i][j]) {
                SDL_RenderCopy(renderer, BLK, NULL, &imgDst);
            }
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            imgDst.x = BLK_SIZE * (curTtm.col + j);
            imgDst.y = BLK_SIZE * (curTtm.row + i);
            if (curTtm.mat[i][j]) {
                SDL_RenderCopy(renderer, BLK, NULL, &imgDst);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int game(void) {
    int i;
    short timer;
    int actions[1000];
    int actionsNr;
    int quit = 0;
    int gameOver = 0;
    char msg[256];

    randTtm();

    timer = 0;
    while (quit != 1) {
        refreshScreen();

        actionsNr = getActions(actions);
        for (i = 0; i < actionsNr; i++) {
            switch (actions[i])
            {
                case ROTATE:
                    rotateCur();
                    break;
                case GO_DOWN:
                    fullDownTtm();
                    break;
                case GO_RIGHT:
                    moveTtm(RIGHT);
                    break;
                case GO_LEFT:
                    moveTtm(LEFT);
                    break;
                case QUIT:
                    quit = 1;
                    break;
            }
        }

        for (i = 0; i < HEIGHT; ++i)
        {
            if (wholeLine(i))
                removeLine(i);
        }

        if (curTtm.printed == 1) {
            if (randTtm() != 0) {
                gameOver = 1;
                quit = 1;
            }
        }

        SDL_Delay(10);
        timer++;
        if (timer == 50) {
            timer = 0;
            downTtm();
        }
    }

    if (gameOver) {
        sprintf(msg, "your score: %li", score);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Over!", msg, NULL);
    }

    return 0;
}

int init(SDL_Window *window) {
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL init error", SDL_GetError(), NULL);
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "make window error", SDL_GetError(), NULL);
        return 1;
    }

    BLK = loadBlk(renderer);
    if (BLK == NULL) {
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    int error_occured;

    srand(time(NULL));

    error_occured = init(window);

    if (error_occured == 0)
        game();

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return error_occured;
}
