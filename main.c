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

char IBlk[][4] = {
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 }
};

char LBlk[][4] = {
    { 0, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 1, 1, 0 }
};

char JBlk[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 1, 0 },
    { 0, 1, 1, 0 }
};

char TBlk[][4] = {
    { 0, 0, 0, 0 },
    { 0, 1, 1, 1 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 }
};

char OBlk[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 1, 1, 0 }
};

char ZBlk[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 0, 0, 1, 1 }
};

char SBlk[][4] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 1, 1, 0 },
    { 1, 1, 0, 0 }
};

SDL_Color white = { 255, 255, 255, 255 };
SDL_Color black = { 0, 0, 0, 0 };

struct blk {
    char mat[4][4];
    int row;
    int col;
};

struct blk curBlk;
char board[HEIGHT][WIDTH] = { 0 };
int noCurBlk = 1;
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

/* this function return 1 if it's possible to print the blk and 0 else */
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
            tmp[i][j] = curBlk.mat[3 - j][i];
        }
    }

    if (canPrint(tmp, curBlk.row, curBlk.col)) {
        memcpy(curBlk.mat, tmp, 4 * 4 * sizeof(char));
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

int randBlk(void) {
    int choice;
    char (*newBlk)[4];

    curBlk.row = 0;
    curBlk.col = 4;

    choice = rand() % 7;
    switch (choice)
    {
        case 0:
            newBlk = IBlk;
            break;
        case 1:
            newBlk = LBlk;
            break;
        case 2:
            newBlk = JBlk;
            break;
        case 3:
            newBlk = TBlk;
            break;
        case 4:
            newBlk = OBlk;
            break;
        case 5:
            newBlk = SBlk;
            break;
        case 6:
            newBlk = ZBlk;
            break;
    }
    memcpy(curBlk.mat, newBlk, 4 * 4 * sizeof(char));

    noCurBlk = 0;

    if (! canPrint(curBlk.mat, curBlk.row, curBlk.col)) { /* game over! */
        return 1;
    }
    return 0;
}

/* this function put a blk in the board */
int printBlk(char mat[4][4], int row, int col) {
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

int downBlk(void) {
    curBlk.row++;
    if (! canPrint(curBlk.mat, curBlk.row + 1, curBlk.col)) {
        printBlk(curBlk.mat, curBlk.row, curBlk.col);
        noCurBlk = 1;
    }
    return 0;
}

void fullDownBlk(void) {
    while (noCurBlk != 1)
        downBlk();
}

void moveBlk(int sens) {
    if (sens == LEFT) {
        if (canPrint(curBlk.mat, curBlk.row, curBlk.col - 1))
            curBlk.col--;
    }
    else if (sens == RIGHT) {
        if (canPrint(curBlk.mat, curBlk.row, curBlk.col + 1))
            curBlk.col++;
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
    char newBoard[HEIGHT][WIDTH] = { 0 };

    for (i = 0; i < row; i++) {
        for (j = 0; j < WIDTH; j++) {
            board[row - i][j] = board[row-1 - i][j];
        }
    }

    score += 100;
}

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
            imgDst.x = BLK_SIZE * (curBlk.col + j);
            imgDst.y = BLK_SIZE * (curBlk.row + i);
            if (curBlk.mat[i][j]) {
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

    randBlk();

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
                    fullDownBlk();
                    break;
                case GO_RIGHT:
                    moveBlk(RIGHT);
                    break;
                case GO_LEFT:
                    moveBlk(LEFT);
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

        if (noCurBlk) {
            if (randBlk() != 0) {
                gameOver = 1;
                quit = 1;
            }
        }

        SDL_Delay(10);
        timer++;
        if (timer == 50) {
            timer = 0;
            downBlk();
        }
    }

    if (gameOver) {
        sprintf(msg, "your score: %li", score);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Over!", msg, NULL);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    int state = 1;
    SDL_Surface *tmpBlk = NULL;
    SDL_Window *window;

    srand(time(NULL));

    /* ======================================================== */
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL init error", SDL_GetError(), NULL);
        goto Quit;
    }

    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "make window error", SDL_GetError(), NULL);
        goto Quit;
    }

    BLK = loadBlk(renderer);
    if (BLK == NULL) {
        goto Quit;
    }
    /* ======================================================== */

    game();

    state = 0;

Quit:
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return state;
}
