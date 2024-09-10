#include "SDL2/SDL.h"

#ifndef TABLE_GUARD
#include "../table.h"
#endif

#ifndef BLOCK_GUARD
#include "../block.h"
#endif


#define LIVING_GUARD 1

class Living{
    public:
        int width = 16;
        int height = 16;
        bool dead = false;
        bool falls = true;
        bool jumping;
        bool jump;
        double x;
        double y;
        double speed_x = 0.0;
        double speed_y = 0.0;
        int coyote_time = -1;
        int animation = 0;
        int sub_animation = 0;
        int id;
        int tempInt = 0;
        bool tempBool = false;
        SDL_Rect hit;
        SDL_Rect texture_source;

        void make();

        void update(Table table);
        void render(SDL_Renderer *renderer, SDL_Texture *sheet, int off_x, int off_y);

        void move();
        void move(Table table);
        
        double distanceUp(Table table);
        double distanceDown(Table table);
        double distanceLeft(Table table);
        double distanceRight(Table table);
        bool nextDiagonalBlock(Table table);
        bool nextDiagonalBlock(Table table, bool x, bool y);

    protected:
        virtual void Specific_update(Table table) = 0;
};