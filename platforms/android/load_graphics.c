#include <stdlib.h>

#include "../../sdl.h"

extern char **xpmBlk;

SDL_Texture *loadBlk(SDL_Renderer *renderer) {
	SDL_Surface *tmpBlk;
	SDL_Texture *BLK;

	tmpBlk = IMG_ReadXPMFromArray(xpmBlk);
	BLK = SDL_CreateTextureFromSurface(renderer, tmpBlk);
	SDL_FreeSurface(tmpBlk);

	return BLK;
}
