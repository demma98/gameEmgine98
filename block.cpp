#include "block.h"

void Block :: drawBlock(SDL_Renderer *renderer, int x, int y, int id, const char *path){
    if(id != 0){
        SDL_Texture *draw = IMG_LoadTexture(renderer, path);
        SDL_Rect source;
        source.x = (id % 8) * width;
        source.y = (id / 8) * height;
        source.w = width;
        source.h = height;
        
        SDL_Rect destiny = source;
        destiny.x = x * width;
        destiny.y = y * height;

        SDL_RenderCopy(renderer, draw, &source, &destiny);
    }
}