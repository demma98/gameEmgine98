#include "SDL2/SDL.h"

#ifndef TABLE_GUARD
#include "table.h"
#endif

#ifndef BLOCK_GUARD
#include "block.h"
#endif


#define LEVEL_GUARD 1

class Living{
    public:
        int width = 16;
        int height = 16;
        bool dead;
        bool falls;
        double x;
        double y;
        double speed_x;
        double speed_y;
        int animation;
        int sub_animation;
        int id;
        SDL_Rect hit;
        SDL_Rect texture_source;

        Living(int x, int y, int id);
        Living() = default;
        void update(Table table);
        void render(SDL_Renderer *renderer, SDL_Texture *sheet, int off_x, int off_y);

        void move();
        void move(Table table);
};