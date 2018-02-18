#include <stdlib.h>

#include "../../sdl.h"
#include "../../main.h"

int getActions(int *actions) {
    SDL_Event event;
    int actionsNr = 0;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_FINGERDOWN) {
            if (event.tfinger.dy < -30) {
                actions[actionsNr] = ROTATE;
                actionsNr++;
            }
            else if (event.tfinger.dy > 30) {
                actions[actionsNr] = GO_DOWN;
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
        
        if (event.type == SDL_QUIT) {
            actions[actionsNr] = QUIT;
            actionsNr++;
        }
    }
    return actionsNr;
}