#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define BLOCK_GUARD 1

class Block{
    public :
        int width = 16;
        int height = 16;

        void drawBlock(SDL_Renderer *renderer, int x, int y, int id, const char *path);

};