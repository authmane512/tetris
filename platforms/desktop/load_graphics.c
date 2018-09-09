/**
 * @file platforms/desktop/load_graphics.c
 */

#include <stdlib.h>

#include "../../sdl.h"

extern char **xpmBlk;

/**
 * @brief      Gets block texture
 *
 * @param      renderer  The SDL renderer
 *
 * @return     SDL texture of the red block
 */
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
