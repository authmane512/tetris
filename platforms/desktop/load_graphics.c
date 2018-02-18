#include <stdlib.h>

#include "../../sdl.h"

extern char **xpmBlk;

SDL_Texture *loadBlk(SDL_Renderer *renderer) {
	SDL_Surface *tmpBlk;
	SDL_Texture *BLK;

	tmpBlk = SDL_LoadBMP("red_block.bmp");
	if (NULL == tmpBlk)
	{
	    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error SDL_LoadBMP", SDL_GetError(), NULL);
	    return NULL;
	}

	BLK = SDL_CreateTextureFromSurface(renderer, tmpBlk);
	SDL_FreeSurface(tmpBlk);

	return BLK;
}
