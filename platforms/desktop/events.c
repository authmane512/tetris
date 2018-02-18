#include <stdlib.h>

#include "../../sdl.h"
#include "../../main.h"

int getActions(int *actions) {
    SDL_Event event;
    int actionsNr = 0;

    while (SDL_PollEvent(&event)) {
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

        if (event.type == SDL_QUIT) {
            actions[actionsNr] = QUIT;
            actionsNr++;
        }
    }
    return actionsNr;
}