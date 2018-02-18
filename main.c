#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#ifdef __ANDROID__
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#else
#include <stdio.h>
#include <SDL2/SDL.h>
#endif

#define HEIGHT 18
#define WIDTH 12
#define BLK_SIZE 32

#define SCREEN_HEIGHT BLK_SIZE * HEIGHT
#define SCREEN_WIDTH BLK_SIZE * WIDTH

#ifdef __ANDROID__
static char *xpmBlk[] = {
"32 32 51 1 ",
"  c #8C0707",
". c #8B0808",
"X c #8B0A0A",
"o c #8D0A0A",
"O c #8E0B0B",
"+ c #BE0E0E",
"@ c #D20F0F",
"# c #DE1010",
"$ c #DE1111",
"% c #DE1212",
"& c #DE1414",
"* c #DE1515",
"= c #DF1717",
"- c #DF1A1A",
"; c #DF1D1D",
": c #E01F1F",
"> c #E02020",
", c #E02222",
"< c #E12525",
"1 c #E12828",
"2 c #E12929",
"3 c #E12A2A",
"4 c #E12B2B",
"5 c #E22D2D",
"6 c #E22F2F",
"7 c #E23232",
"8 c #E33535",
"9 c #E33838",
"0 c #E43B3B",
"q c #F23737",
"w c #EB4C4C",
"e c #EB4D4D",
"r c #EB4F4F",
"t c #F24444",
"y c #F34646",
"u c #F34949",
"i c #F34B4B",
"p c #F34C4C",
"a c #F34F4F",
"s c #EC5151",
"d c #EC5454",
"f c #EC5555",
"g c #F35151",
"h c #F45454",
"j c #F47171",
"k c #F47272",
"l c #F47373",
"z c #F47575",
"x c #F47777",
"c c #F47878",
"v c None",
"voooOOOOOOOOOOOOOOOOOOOOOOOOOOov",
"ooo.OOOOOoOOOOOOOOOOOOOOOOOOOOO ",
"oOejkkkkkkkkkjjjjjjjkjkkjjjjjtOO",
"oo:tkkkkkkkkkkkkkkkkkkkkkkkkw:OO",
"oo=%ekkzkzkkkzkzkkzkkzzkllle$$.O",
"oo=-=szzzzzzzzzzzzzzzzzzlls-==OO",
"OO----sccczczcczzczczczccf----OO",
"OO;;;;;fccccccczccccccccf;;;;;OO",
"OO>:>::>yyttyytttttttttt>>>>>;OO",
"OO>>>>>>yyyyyyyyyyyttttt<>>>><OO",
"OO<<<<<<uuuuuuuuuuiiiiii<<<<<<OO",
"OO331111pupuupppuuiiiiii111111OO",
"OO333341ppppppppppppaiii111111OO",
"OO666464pgpgggpggppgaaaa666666OO",
"OO666666gggggggggggggggg666667OO",
"OO777777hhhhghhhhhhhhhhh777777OO",
"OO88887:qqqqqqqqqqqqqqqq>88888oO",
"Oo9983=#qqqqqqqqqqqqqqqq#=6889OO",
"OO98;%##qqqqqqqqqqqqqqqq###;89OO",
"OO3%####qqqqqqqqqqqqqqqq####=3OO",
"OO%%####qqqqqqqqqqqqqqqq######oo",
"OO######qqqqqqqqqqqqqqqq######Oo",
"oo####%%qqqqqqqqqqqqqqqq#####%OO",
"Oo#####%qqqqqqqqqqqqqqqq#####%OO",
"OO#####@++++++++++++++++@#####oO",
"OO####@++++++++++++++++++@####OO",
"OO:##@++++++++++++++++++++@###OO",
"OO##@++++++++++++++++++++++@##OO",
"oo@@++++++++++++++++++++++++@@.O",
"oo@@++++++++++++++++++++++++@@OO",
"ooOOOO  OOOOOOOOOOOOOOOOOOOOOOO.",
"v OOOOOOOOOOOOOOOOOOOOOOOOOOOO.v"
};
#endif

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

enum { LEFT, RIGHT };
enum { GO_LEFT, GO_RIGHT, ROTATE, GO_DOWN, QUIT };

struct blk curBlk;
char board[HEIGHT][WIDTH] = { 0 };
int noCurBlk = 1;
long score = 0;
SDL_Texture *BLK;
SDL_Renderer *renderer;

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

int getActions(int *actions) {
    SDL_Event event;
    int actionsNr = 0;

    while (SDL_PollEvent(&event)) {
        #ifdef __ANDROID__
        if (event.type == SDL_FINGERDOWN) {
            if (event.tfinger.dy < -30) {
                actions[actionsNr] = ROTATECur;
                actionsNr++;
            }
            else if (event.tfinger.dy > 30) {
                factions[actionsNr] = GO_DOWN;
                actionsNr++;
            }
            else if (event.tfinger.x > 12345) { 
                actions[actionsNr] = GO_RIGHT;
                actionsNr++;
            }
            else if (event.tfinger.x < 12345) {
                actions[actionsNr] = GO_LEFT;
                actionsNr++;
            }
        }
        #else
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                    actions[actionsNr] = ROTATE;
                    actionsNr++;
                    break;
                case SDLK_DOWN:
                    actions[actionsNr] = GO_DOWN;
                    actionsNr++;
                    break;
                case SDLK_RIGHT:
                    actions[actionsNr] = GO_RIGHT;
                    actionsNr++;
                    break;
                case SDLK_LEFT:
                    actions[actionsNr] = GO_LEFT;
                    actionsNr++;
                    break;
                case SDLK_q:
                    actions[actionsNr] = QUIT;
                    actionsNr++;
                    break;
            }
        }
        #endif
        if (event.type == SDL_QUIT) {
            actions[actionsNr] = QUIT;
            actionsNr++;
        }
    }
    return actionsNr;
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

    #ifdef __ANDROID__
    tmpBlk = IMG_ReadXPMFromArray(xpmBlk);
    #else
    tmpBlk = SDL_LoadBMP("red_block.bmp");
    if(NULL == tmpBlk)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error SDL_LoadBMP", SDL_GetError(), NULL);
        goto Quit;
    }
    #endif

    BLK = SDL_CreateTextureFromSurface(renderer, tmpBlk);
    SDL_FreeSurface(tmpBlk);
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
