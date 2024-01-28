#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

#ifndef DISK_GUARD
#include "disk.h"
#endif

#ifndef BLOCK_GUARD
#include "block.h"
#endif

#define TABLE_GUARD 1

class Table {
    public :
        int width;
        int height;
        double off_x;
        double off_y;
        int *blocks;
        int max_block;
        const char *path;
        const char *sheet_path;
        SDL_Texture *image;

        Table(SDL_Renderer *renderer, int max_block, const char *path, const char *sheet_path);
        Table() = default;
        void load();
        void save();
        void update(SDL_Renderer *renderer);
        void render(SDL_Renderer *renderer);
        void destroy();

        void addRows(SDL_Renderer *renderer, int n);
        void addColumns(SDL_Renderer *renderer, int n);
        void changeBlock(SDL_Renderer *renderer, int x, int y, int n, bool save_change);
        void setBlock(SDL_Renderer *renderer, int x, int y, int n, bool save_change);
        void flipVertical(SDL_Renderer *renderer, bool save_change);
        void flipHorizontal(SDL_Renderer *renderer, bool save_change);
};